#!/bin/bash
INSTALL_PATH=/usr/local/opencctv

color_green='\033[0;32m'
color_blue='\033[0;36m'
color_reset='\033[0m'

sudo mkdir -p ${INSTALL_PATH}
sudo chmod -R 777 ${INSTALL_PATH}
echo -e "${color_green}Installation directory created at $INSTALL_PATH.${color_reset}"

# Installing AnalyticStarter
echo -e "${color_green}*Building AnalyticStarter...${color_reset}"
cd AnalyticStarter/Release/
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

echo -e "${color_green}Analytic Server installation complete.${color_reset}"
