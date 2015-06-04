class Camera < ActiveRecord::Base
  belongs_to :vms
  has_many :streams, dependent: :destroy

  # validates the milestone Camera
  # returns true if valid and false if invalid
  def milestone_validate_camera
    xml_builder = Nokogiri::XML::Builder.new do |xml|
      xml.send(:'methodcall') {
        xml.send(:'requestid', '1')
        xml.send(:'methodname', 'connect')
        xml.send(:'username', self.vms.username)
        xml.send(:'password', self.vms.password)
        xml.send(:'cameraid', self.camera_id)
        xml.send(:'alwaysstdjpeg', 'yes')
      }
    end
    return send_receive_validate_connection(self.vms, xml_builder)
  end

  def milestone_grab_default_stream_frame
    complete = false
    width = -1
    height = -1
    if(!self.errors.any?)
      img_path = "#{Rails.root}/app/assets/images/#{self.vms.id}_#{self.id}.jpeg"
      cmd = "java -jar #{Rails.root}/app/assets/programs/MilestoneFrameGrabber/mx_grab_stream_frame.jar #{self.vms.server_ip} #{self.vms.server_port} #{self.vms.username} #{self.vms.password} #{self.camera_id} #{img_path}"
      stdin, stdout, stderr = Open3.popen3(cmd)
      output = stdout.gets
      if(!output.nil? && output.start_with?("<"))
        doc = Nokogiri::XML(output)
        error = doc.xpath('//output//error')[0].content
        if(!error.nil? && error == "no")
          width = doc.xpath('//output//width')[0].content.to_i
          height = doc.xpath('//output//height')[0].content.to_i
          complete = true
        end
      end
    end
    return complete, width, height
  end

  def zoneminder_grab_default_frame(streamUrl)
    complete = false
    width = -1
    height = -1
    img_path = "#{Rails.root}/app/assets/images/#{self.vms.id}_#{self.id}.jpeg"
    cmd = "java -jar #{Rails.root}/app/assets/programs/ZoneminderFrameGrabber/zm_grab_stream_frame.jar #{streamUrl} #{img_path}"
    stdin, stdout, stderr = Open3.popen3(cmd)
    output = stdout.readline
    if(!output.nil? && output.start_with?("<"))
      error = doc.xpath('//output//error')[0].content
      if(!error.nil? && error == "no")
        width = doc.xpath('//output//width')[0].content.to_i
        height = doc.xpath('//output//height')[0].content.to_i
        complete = true
      end
    end
    return complete, width, height
  end

  def set_verification(is_verified)
    if(!is_verified)
      self.streams.each do |stream|
        stream.set_verification(false)
      end
    end
    update(:verified => is_verified)
  end

  private
  def send_receive_validate_connection(vms, connect_xml_builder)
    if(vms.errors.any?)
      return false
    end
    msg = connect_xml_builder.to_xml.to_s + "\r\n\r\n"
    sock = TCPSocket.new( vms.server_ip, vms.server_port )
    sock.write( msg )
    reply = sock.recv( 2048 )
    if(reply.start_with?("<"))
      doc = Nokogiri::XML(reply)
      connected = doc.xpath('//methodresponse//connected')[0].content
      if(connected == "yes")
        return true
      end
    end
    return false
  end
end