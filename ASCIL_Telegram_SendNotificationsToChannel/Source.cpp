#define CURL_STATICLIB
#include "sierrachart.h"
#include <iostream>
#include <string>
#include "curl/curl.h"

#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")


SCDLLName("Custom Study DLL")


const std::string token = "TOKEN";
const std::string chatId = "CHAT_ID"; // Group chat ID or user chat ID

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


SCSFExport scsf_UniqueFunctionName(SCStudyInterfaceRef sc)
{
    if (sc.SetDefaults)
    {
        sc.GraphName = "My New Study Function";
        sc.Subgraph[0].Name = "Subgraph name";
        sc.Subgraph[0].DrawStyle = DRAWSTYLE_LINE;

        sc.AutoLoop = 1;

        return;
    }

    sc.Subgraph[0][sc.Index] = sc.Close[sc.Index] * 10;

    SCDateTime CurrentDateTime;
    if (sc.IsReplayRunning())	CurrentDateTime = sc.CurrentDateTimeForReplay;
    else						CurrentDateTime = sc.CurrentSystemDateTime;

    int Year, Month, Day, Hour, Minute, Second;
    CurrentDateTime.GetDateTimeYMDHMS(Year, Month, Day, Hour, Minute, Second);

    if (Second == 0 || Second == 10 || Second == 20 || Second == 30 || Second == 40 || Second == 50) {
        CURL* curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();

        if (curl) {
            std::string message = "Hello, Telegram Group!";
            std::string encodedMessage;

            // URL-encode the message
            CURL* handle = curl_easy_init();
            if (handle) {
                encodedMessage = curl_easy_escape(handle, message.c_str(), message.length());
                curl_easy_cleanup(handle);
            }

            std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";
            url += "?chat_id=" + chatId + "&text=" + encodedMessage;

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &message);

            res = curl_easy_perform(curl);
            SCString Buffer;
            if (res != CURLE_OK) {
                //std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                Buffer.Format("   sc.Index: %d       res != CURLE_OK                                                                                                                                                                                                                                                                                                                                                                                     ",
                    sc.Index);						sc.AddMessageToLog(Buffer, 1);
            }

            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
    }

    return;
}