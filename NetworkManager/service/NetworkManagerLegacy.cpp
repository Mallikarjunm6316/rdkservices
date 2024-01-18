/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2022 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#include "NetworkManager.h"

using namespace WPEFramework::Plugin;

namespace WPEFramework
{
    namespace Plugin
    {
        /**
         * Hook up all our JSON RPC methods
         *
         * Each method definition comprises of:
         *  * Input parameters
         *  * Output parameters
         *  * Method name
         *  * Function that implements that method
         */
        void NetworkManager::RegisterLegacyMethods()
        {
            Register("getInterfaces",                     &NetworkManager::getInterfaces, this);
            Register("isInterfaceEnabled",                &NetworkManager::isInterfaceEnabled, this);
            Register("getPublicIP",                       &NetworkManager::getPublicIP, this);
            Register("setInterfaceEnabled",               &NetworkManager::setInterfaceEnabled, this);
            Register("getDefaultInterface",               &NetworkManager::getDefaultInterface, this);
            Register("setDefaultInterface",               &NetworkManager::setDefaultInterface, this);
            Register("setIPSettings",                     &NetworkManager::setIPSettings, this);
            Register("getIPSettings",                     &NetworkManager::getIPSettings, this);
            Register("getInternetConnectionState",        &NetworkManager::getInternetConnectionState, this);
            Register("ping",                              &NetworkManager::ping, this);
            Register("isConnectedToInternet",             &NetworkManager::isConnectedToInternet, this);
            Register("setConnectivityTestEndpoints",      &NetworkManager::SetConnectivityTestEndpoints, this);
            Register("startConnectivityMonitoring",       &NetworkManager::StartConnectivityMonitoring, this);
            Register("getCaptivePortalURI",               &NetworkManager::GetCaptivePortalURI, this);
            Register("stopConnectivityMonitoring",        &NetworkManager::StopConnectivityMonitoring, this);
            Register("cancelWPSPairing",                  &NetworkManager::StopWPS, this);
            Register("clearSSID",                         &NetworkManager::clearSSID, this);
            Register("connect",                           &NetworkManager::WiFiConnect, this);
            Register("disconnect",                        &NetworkManager::WiFiDisconnect, this);
            Register("getConnectedSSID",                  &NetworkManager::getConnectedSSID, this);
            Register("startScan",                         &NetworkManager::StartWiFiScan, this);
            Register("stopScan",                          &NetworkManager::StopWiFiScan, this);
            Register("getPairedSSID",                     &NetworkManager::GetKnownSSIDs, this);
            Register("getPairedSSIDInfo",                 &NetworkManager::GetConnectedSSID, this);
            Register("initiateWPSPairing",                &NetworkManager::StartWPS, this);
            Register("isPaired",                          &NetworkManager::isPaired, this);
            Register("saveSSID",                          &NetworkManager::AddToKnownSSIDs, this);
            Register("getSupportedSecurityModes",         &NetworkManager::GetSupportedSecurityModes, this);
        }

        /**
         * Unregister all our JSON-RPC methods
         */
        void NetworkManager::UnregisterLegacyMethods()
        {
            Unregister("getInterfaces");
            Unregister("isInterfaceEnabled");
            Unregister("getPublicIP");
            Unregister("setInterfaceEnabled");
            Unregister("getDefaultInterface");
            Unregister("setDefaultInterface");
            Unregister("setIPSettings");
            Unregister("getIPSettings");
            Unregister("getInternetConnectionState");
            Unregister("ping");
            Unregister("isConnectedToInternet");
            Unregister("setConnectivityTestEndpoints");
            Unregister("startConnectivityMonitoring");
            Unregister("getCaptivePortalURI");
            Unregister("stopConnectivityMonitoring");
            Unregister("cancelWPSPairing");
            Unregister("clearSSID");
            Unregister("connect");
            Unregister("disconnect");
            Unregister("getConnectedSSID");
            Unregister("startScan");
            Unregister("stopScan");
            Unregister("getPairedSSID");
            Unregister("getPairedSSIDInfo");
            Unregister("initiateWPSPairing");
            Unregister("isPaired");
            Unregister("saveSSID");
            Unregister("getSupportedSecurityModes");
        }

#define CIDR_NETMASK_IP_LEN 32
const string CIDR_PREFIXES[CIDR_NETMASK_IP_LEN] = {
                                                     "128.0.0.0",
                                                     "192.0.0.0",
                                                     "224.0.0.0",
                                                     "240.0.0.0",
                                                     "248.0.0.0",
                                                     "252.0.0.0",
                                                     "254.0.0.0",
                                                     "255.0.0.0",
                                                     "255.128.0.0",
                                                     "255.192.0.0",
                                                     "255.224.0.0",
                                                     "255.240.0.0",
                                                     "255.248.0.0",
                                                     "255.252.0.0",
                                                     "255.254.0.0",
                                                     "255.255.0.0",
                                                     "255.255.128.0",
                                                     "255.255.192.0",
                                                     "255.255.224.0",
                                                     "255.255.240.0",
                                                     "255.255.248.0",
                                                     "255.255.252.0",
                                                     "255.255.254.0",
                                                     "255.255.255.0",
                                                     "255.255.255.128",
                                                     "255.255.255.192",
                                                     "255.255.255.224",
                                                     "255.255.255.240",
                                                     "255.255.255.248",
                                                     "255.255.255.252",
                                                     "255.255.255.254",
                                                     "255.255.255.255",
                                                   };

        uint32_t NetworkManager::getInterfaces (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpResponse;
            JsonArray array;

            rc = GetAvailableInterfaces(parameters, tmpResponse);
           
            const JsonArray& tmpArray = tmpResponse["interfaces"].Array();
            for (int i=0; i<tmpArray.Length(); i++)
            {
                JsonObject each;
                const JsonObject& arrayEntry = tmpArray[i].Object();
                each[_T("interface")] = arrayEntry["type"];
                each[_T("macAddress")] = arrayEntry["mac"];
                each[_T("enabled")] = arrayEntry["isEnabled"];
                each[_T("connected")] = arrayEntry["isConnected"];

                array.Add(JsonValue(each));
            }

            response["Interfaces"] = array;
            response["success"] = tmpResponse["success"];
            return rc;
        }
        uint32_t NetworkManager::setInterfaceEnabled (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            string interface;
            JsonObject tmpParameters;
            
            if("WIFI" == parameters["interface"].String())
                interface = "wlan0";
            else if("ETHERNET" == parameters["interface"].String())
                interface = "eth0";
            
            tmpParameters["interface"] = interface;
            tmpParameters["enabled"] = parameters["enabled"];
            rc = SetInterfaceEnabled(tmpParameters, response);

            return rc;
        }
        uint32_t NetworkManager::getDefaultInterface (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpResponse;

            rc = GetPrimaryInterface(parameters, tmpResponse);

            if (Core::ERROR_NONE == rc)
            {
                if ("wlan0" == tmpResponse["interface"].String())
                    response["interface"] = "WIFI";
                else if("eth0" == tmpResponse["interface"].String())     
                    response["interface"] = "ETHERNET";
                response["success"] = tmpResponse["success"];
            }
            return rc;
        }
        uint32_t NetworkManager::setDefaultInterface(const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpParameters;
            string interface;
            if("WIFI" == parameters["interface"].String())
                tmpParameters["interface"] = "wlan0";
            else if("ETHERNET" == parameters["interface"].String())
                tmpParameters["interface"] = "eth0";
            
            rc = SetPrimaryInterface(tmpParameters, response);

            return rc;
        }
        uint32_t NetworkManager::setIPSettings(const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpResponse;
            JsonObject tmpParameters;
            Exchange::INetworkManager::IPAddressInfo result{};
            
            if("WIFI" == parameters["interface"].String())
                tmpParameters["interface"] = "wlan0";
            else if("ETHERNET" == parameters["interface"].String())
                tmpParameters["interface"] = "eth0";
            
            tmpParameters["ipversion"] = parameters["ipversion"];
            tmpParameters["autoconfig"] = parameters["autoconfig"];
            tmpParameters["ipaddress"] = parameters["ipaddr"];
            auto it = std::find(std::begin(CIDR_PREFIXES), std::end(CIDR_PREFIXES), parameters["netmask"].String());
            if (it == std::end(CIDR_PREFIXES))
                return rc;
            else
                tmpParameters["prefix"] = std::distance(std::begin(CIDR_PREFIXES), it);
            tmpParameters["gateway"] = parameters["gateway"];
            tmpParameters["primarydns"] = parameters["primarydns"];
            tmpParameters["secondarydns"] = parameters["secondarydns"];

            rc = SetIPSettings(tmpParameters, tmpResponse);

            if (Core::ERROR_NONE == rc)
            {
                response["supported"] = true;
                response["success"] = tmpResponse["success"];
            }
            return rc;
        }
        uint32_t NetworkManager::getIPSettings (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            string interface;
            JsonObject tmpResponse;
            JsonObject tmpParameters;
            size_t index;

            tmpParameters["ipversion"] = parameters["ipversion"];
            if ("WIFI" == parameters["interface"].String())
                tmpParameters["interface"] = "wlan0";
            else if("ETHERNET" == parameters["interface"].String())
                tmpParameters["interface"] = "eth0";

            rc = GetIPSettings(tmpParameters, tmpResponse); 

            if (Core::ERROR_NONE == rc)
            {
                index = tmpResponse["prefix"].Number();
                if(CIDR_NETMASK_IP_LEN <= index)
                    return Core::ERROR_GENERAL;
                else
                    response["netmask"]  = CIDR_PREFIXES[index];
                response["interface"]    = parameters["interface"];
                response["ipversion"]    = tmpResponse["ipversion"];
                response["autoconfig"]   = tmpResponse["autoconfig"];
                response["dhcpserver"]   = tmpResponse["dhcpserver"];
                response["ipaddr"]       = tmpResponse["ipaddress"];
                response["gateway"]      = tmpResponse["gateway"];
                response["primarydns"]   = tmpResponse["primarydns"];
                response["secondarydns"] = tmpResponse["secondarydns"];
                response["success"]      = tmpResponse["success"];
            }
            return rc;
        }
        uint32_t NetworkManager::isConnectedToInternet(const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpResponse;

            string ipversion = parameters["ipversion"].String();
            rc = IsConnectedToInternet(parameters, tmpResponse);

            if (Core::ERROR_NONE == rc)
            {
                response["connectedToInternet"] = tmpResponse["isConnectedToInternet"];
                response["ipversion"] = ipversion.c_str();
                response["success"] = true;
            }
            return rc;
        }
        uint32_t NetworkManager::getInternetConnectionState(const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            uint32_t rc1 = Core::ERROR_GENERAL;
            string endPoint;
            JsonObject tmpResponse;
            JsonObject captivePortalResponse;
            JsonObject tmpParameters;
            string ipversion = parameters["ipversion"].String();
            rc = IsConnectedToInternet(parameters, tmpResponse);
            if (Core::ERROR_NONE == rc)
            {
                response["state"] = tmpResponse["internetState"];
                if (response["state"].Number() == 2)
                {
                    rc1 = getCaptivePortalURI(tmpParameters, captivePortalResponse);

                    if (Core::ERROR_NONE == rc1)
                        response["uri"] = captivePortalResponse["uri"];
                }
                response["ipversion"] = ipversion.c_str();
                response["success"] = true;
            }
            return rc;
        }
        uint32_t NetworkManager::ping(const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            struct in_addr ipv4address;
            struct in6_addr ipv6address;
            JsonObject tmpParameters;
            JsonObject tmpResponse;
            string endpoint{};

            endpoint = parameters["endpoint"].String();

            if (inet_pton(AF_INET, endpoint.c_str(), &ipv4address) > 0)
                tmpParameters["ipversion"] = "IPv4";
            else if (inet_pton(AF_INET6, endpoint.c_str(), &ipv6address) > 0)
                tmpParameters["ipversion"] = "IPv6";
            
            tmpParameters["noOfRequest"] = parameters["packets"];
            tmpParameters["endpoint"] = parameters["endpoint"];
            tmpParameters["timeout"] = 5;
            tmpParameters["guid"] = parameters["guid"];
            
            rc = Ping(tmpParameters, response);

            if (Core::ERROR_NONE == rc)
            {
                response["success"] = true;
            }
            return rc;
        }
        uint32_t NetworkManager::getPublicIP(const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            string interface;
            string ipversion{"IPv4"};
            JsonObject tmpParameters;
            JsonObject tmpResponse;

            if("WIFI" == parameters["iface"].String())
                interface = "wlan0";
            else if("ETHERNET" == parameters["iface"].String())
                interface = "eth0";

             if(parameters["ipv6"].Boolean())
                ipversion = "IPv6";
            else
                ipversion = "IPv4";

            tmpParameters["interface"] = interface;
            tmpParameters["ipversion"] = ipversion;
            rc = GetPublicIP(tmpParameters, tmpResponse);

            if (Core::ERROR_NONE == rc)
            {
                response["public_ip"]    = tmpResponse["publicIP"];
                response["success"]      = tmpResponse["success"];
            }
            return rc;
        }

        uint32_t NetworkManager::isInterfaceEnabled (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpResponse;

            rc = GetAvailableInterfaces(parameters, tmpResponse);
          
            const JsonArray& tmpArray = tmpResponse["interfaces"].Array();
            for (int i=0; i<tmpArray.Length(); i++)
            {
                JsonObject each;
                const JsonObject& arrayEntry = tmpArray[i].Object();
                if(parameters["interface"].String() == arrayEntry["type"].String())
                {
                    response["enabled"] = arrayEntry["isEnabled"];
                    break;
                }
            }
            response["success"] = tmpResponse["success"];

            return rc;
        }
        uint32_t NetworkManager::clearSSID (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpParameters;

            tmpParameters["ssid"] = "";
            rc = RemoveKnownSSID(tmpParameters, response);

            return rc;
        }
 
        uint32_t NetworkManager::getConnectedSSID (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;
            JsonObject tmpResponse;

            rc = GetConnectedSSID(parameters, tmpResponse);

            if (Core::ERROR_NONE == rc)
            {
                response["ssid"] = tmpResponse["ssid"];
                response["bssid"] = tmpResponse["bssid"];
                response["rate"] = tmpResponse["rate"];
                response["noise"] = tmpResponse["noise"];
                response["security"] = tmpResponse["securityMode"];
                response["signalStrength"] = tmpResponse["signalStrength"];
                response["frequency"] = tmpResponse["frequency"];
                response["success"] = tmpResponse["success"];
            }
            return rc;
        }
        
        uint32_t NetworkManager::isPaired (const JsonObject& parameters, JsonObject& response)
        {
            uint32_t rc = Core::ERROR_GENERAL;

            rc = GetKnownSSIDs(parameters, response);

            if (Core::ERROR_NONE == rc)
            {
                JsonArray array = response["ssids"].Array();
                if (0 == array.Length())
                {
                    response["result"] = 1;
                }
                else
                {
                    response["result"] = 0;
                }
                response["success"] = true;
            }
            return rc;
        }
    }
}
