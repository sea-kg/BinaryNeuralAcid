
#include "argument_processor_main.h"
#include <wsjcpp_core.h>
#include "bna_test_sin.h"

// ---------------------------------------------------------------------
// ArgumentProcessorMain

ArgumentProcessorMain::ArgumentProcessorMain() 
: WsjcppArgumentProcessor({"main"}, 
    "Experiment for reversing md5-hash function",
    "Experiment for reversing md5-hash function") {
    TAG = "ArgumentProcessorMain";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    registryProcessor(new ArgumentProcessorTestSin());
    registryProcessor(new ArgumentProcessorStartServer());
    
}

// ---------------------------------------------------------------------

bool ArgumentProcessorMain::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorMain::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorMain::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    return -1; 
}


// ---------------------------------------------------------------------
// ArgumentProcessorMain

ArgumentProcessorStartServer::ArgumentProcessorStartServer() 
: WsjcppArgumentProcessor({"start-server"}, 
    "Start server",
    "Start server for visualizaion") {
    TAG = "ArgumentProcessorStartServer";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorStartServer::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorStartServer::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorStartServer::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    // "Server starting on 43735 port"
    return -1; 
}


// ---------------------------------------------------------------------
// ArgumentProcessorTestSin

ArgumentProcessorTestSin::ArgumentProcessorTestSin() 
: WsjcppArgumentProcessor({"test-sin"}, 
    "test-sin",
    "test-sin") {
    TAG = "ArgumentProcessorTestSin";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

int ArgumentProcessorTestSin::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");

    BNATestSin testsin;
    testsin.run();

    return -1; 
}