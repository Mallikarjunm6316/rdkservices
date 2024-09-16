#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "L2Tests.h"
#include "L2TestsMock.h"
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <interfaces/ISystemMode.h>

#define TEST_LOG(x, ...) fprintf(stderr, "\033[1;32m[%s:%d](%s)<PID:%d><TID:%d>" x "\n\033[0m", __FILE__, __LINE__, __FUNCTION__, getpid(), gettid(), ##__VA_ARGS__); fflush(stderr);

#define JSON_TIMEOUT   (1000)
#define SYSTEMMODE_CALLSIGN  _T("org.rdk.SystemMode")
#define SYSTEMMODEL2TEST_CALLSIGN _T("L2tests.1")

using ::testing::NiceMock;
using namespace WPEFramework;
using testing::StrictMock;
using ::WPEFramework::Exchange::ISystemMode;


class AsyncHandlerMock
{
    public:
        AsyncHandlerMock()
        {
        }
};

class SystemModeTest : public L2TestMocks {
protected:
    virtual ~SystemModeTest() override;

    public:
    SystemModeTest();

    protected:
        /** @brief Pointer to the IShell interface */
        PluginHost::IShell *m_controller_usersettings;

        /** @brief Pointer to the IUserSettings interface */
        //Exchange::IUserSettings *m_usersettingsplugin;
};

SystemModeTest:: SystemModeTest():L2TestMocks()
{
        Core::JSONRPC::Message message;
        string response;
        uint32_t status = Core::ERROR_GENERAL;

         /* Activate plugin in constructor */
         status = ActivateService("org.rdk.SystemMode");
         EXPECT_EQ(Core::ERROR_NONE, status);
}

/**
 * @brief Destructor for SystemServices L2 test class
 */
SystemModeTest::~SystemModeTest()
{
    uint32_t status = Core::ERROR_GENERAL;

    status = DeactivateService("org.rdk.SystemMode");
    EXPECT_EQ(Core::ERROR_NONE, status);
}

TEST_F(SystemModeTest,GetStatedefault)
{
    JSONRPC::LinkType<Core::JSON::IElement> jsonrpc(SYSTEMMODE_CALLSIGN,SYSTEMMODEL2TEST_CALLSIGN);
    uint32_t status = Core::ERROR_GENERAL;
    JsonObject params;
    JsonObject result;
    std::string message;
    std::string reply;	

    params["systemMode"] = "DeviceOptimize";
    status = InvokeServiceMethod("org.rdk.SystemMode", "GetState", params, result);
    EXPECT_EQ(Core::ERROR_NONE, status);	

}

TEST_F(SystemModeTest,RequestStateGame)
{
    JSONRPC::LinkType<Core::JSON::IElement> jsonrpc(SYSTEMMODE_CALLSIGN,SYSTEMMODEL2TEST_CALLSIGN);
    uint32_t status = Core::ERROR_GENERAL;
    JsonObject params;
    JsonObject result;
    std::string message;
    std::string reply;	

    params["systemMode"] = "DeviceOptimize";
    params["state"]  = "Game";
    status = InvokeServiceMethod("org.rdk.SystemMode", "RequestState", params, result);
    EXPECT_EQ(Core::ERROR_NONE, status);

    status = InvokeServiceMethod("org.rdk.SystemMode", "GetState", params, result);
    EXPECT_EQ(Core::ERROR_NONE, status);
 
}

TEST_F(SystemModeTest,RequestStateVideo)
{
    JSONRPC::LinkType<Core::JSON::IElement> jsonrpc(SYSTEMMODE_CALLSIGN,SYSTEMMODEL2TEST_CALLSIGN);
    uint32_t status = Core::ERROR_GENERAL;
    JsonObject params;
    JsonObject result;
    std::string message;
    std::string reply;	

    params["systemMode"] = "DeviceOptimize";
    params["state"]  = "Video";
    status = InvokeServiceMethod("org.rdk.SystemMode", "RequestState", params, result);
    EXPECT_EQ(Core::ERROR_NONE, status);

    status = InvokeServiceMethod("org.rdk.SystemMode", "GetState", params, result);
    EXPECT_EQ(Core::ERROR_NONE, status);
 
}

