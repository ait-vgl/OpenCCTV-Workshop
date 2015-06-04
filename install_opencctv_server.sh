#!/bin/bash
INSTALL_PATH=/usr/local/opencctv

color_green='\033[0;32m'
color_blue='\033[0;36m'
color_reset='\033[0m'

sudo mkdir -p ${INSTALL_PATH}
sudo chmod -R 777 ${INSTALL_PATH}
echo -e "${color_green}Installation directory created at $INSTALL_PATH.${color_reset}"

# Installing OpenCCTVServer
echo -e "${color_green}*Building OpenCCTVServer...${color_reset}"
cd OpenCCTVServer/Release/
# make clean && make all
make
OPENCCTVSERVER_PATH=${INSTALL_PATH%%/}/OpenCCTVServer/
mkdir -p ${OPENCCTVSERVER_PATH}
#cp OpenCCTVServer ${OPENCCTVSERVER_PATH}
#cp opencctv-server.config ${OPENCCTVSERVER_PATH}
cp {OpenCCTVServer,opencctv-server.config} ${OPENCCTVSERVER_PATH}
cp -a test/ ${OPENCCTVSERVER_PATH}
echo -e "${color_green}Done. OpenCCTVServer installed at $OPENCCTVSERVER_PATH.${color_reset}"

# Installing FakeVmsConnectorPlugin
echo -e "${color_green}*Building FakeVmsConnectorPlugin...${color_reset}"
cd ../../FakeVmsConnectorPlugin/Release/
# make clean && make all
make
FAKECONN_PATH=${INSTALL_PATH%%/}/vms_connectors/FakeVmsConnector
mkdir -p ${FAKECONN_PATH}
cp {libFakeVmsConnectorPlugin.so,analytic.config,sample_large.jpg,sample_mid.jpg,sample_small.jpg} ${FAKECONN_PATH}
echo -e "${color_green}Done. FakeVmsConnectorPlugin installed at $FAKECONN_PATH.${color_reset}"

echo -e "${color_green}OpenCCTV Server installation complete.${color_reset}"
echo -e "${color_blue}Please change the Analytic Server information in opencctv-server.config at $OPENCCTVSERVER_PATH.${color_reset}"
echo -e "\t${color_blue}analytic_server_ip = <your analytic server ip>${color_reset}"
