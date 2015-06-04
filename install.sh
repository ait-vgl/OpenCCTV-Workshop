#!/bin/bash
INSTALL_PATH=/usr/local/opencctv

color_green='\033[0;32m'
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

# Installing AnalyticStarter
echo -e "${color_green}*Building AnalyticStarter...${color_reset}"
cd ../../AnalyticStarter/Release/
# make clean && make all
make
STARTER_PATH=${INSTALL_PATH%%/}/AnalyticStarter
mkdir -p ${STARTER_PATH}
cp {AnalyticStarter,analytic-server.config} ${STARTER_PATH}
echo -e "${color_green}Done. AnalyticStarter installed at $STARTER_PATH.${color_reset}"

# Installing AnalyticRunner
echo -e "${color_green}*Building AnalyticRunner...${color_reset}"
cd ../../AnalyticRunner/Release/
# make clean && make all
make
RUNNER_PATH=${INSTALL_PATH%%/}/AnalyticRunner
mkdir -p ${RUNNER_PATH}
cp AnalyticRunner ${RUNNER_PATH}
echo -e "${color_green}Done. AnalyticRunner installed at $RUNNER_PATH.${color_reset}"

# Installing MockAnalyticPlugin
echo -e "${color_green}*Building MockAnalyticPlugin...${color_reset}"
cd ../../MockAnalyticPlugin/Release/
# make clean && make all
make
MOCK_ANALYTIC_PATH=${INSTALL_PATH%%/}/analytics/MockAnalytic
mkdir -p ${MOCK_ANALYTIC_PATH}
cp libMockAnalyticPlugin.so ${MOCK_ANALYTIC_PATH}
echo -e "${color_green}Done. MockAnalyticPlugin installed at $MOCK_ANALYTIC_PATH.${color_reset}"

echo -e "${color_green}OpenCCTV Server and Analytic Server installation complete.${color_reset}"
