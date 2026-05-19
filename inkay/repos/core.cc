#include "eg2/network/curl.hh"
#include "eg2/sys/report.hh"
#include "eg2/sys/browser.hh"
#include "inkay/repos/core.hh"
#include "inkay/repos/versions.hh"
#include "inkay/utils/json.hh"
#include <string>

namespace Inkay {
    namespace Repos {
        namespace Web {
            std::string JuxtFileURL;
            std::string RoseFileURL = "https://projectrose.cafe/api/inkay-roseverse/latest"; // Rosé provides link
        }

        static const char* JuxtREPO = "https://api.github.com/repos/PretendoNetwork/Inkay/releases/latest";
        static const char* AromaAPI = "https://aroma.foryour.cafe/api/check_versions";
        static const char* BrowserURL = "https://projectrose.cafe/roseverse_installer/help";

        void Init(void) {
            Network::GiveCertCA("fs:/vol/content/shoumeisho/CA.pem");
            Network::SetUserAgent("Mozilla/5.0 (Inkay/1.0; WiiU; curl)");
            Network::SetHeader("Accept: application/vnd.github+json");
            Network::SetHeader("X-GitHub-Api-Version: 2022-11-28");

            Inkay::JSON::ParseGitHubDownloadURL(JuxtREPO, Web::JuxtFileURL);

            Inkay::JSON::ParseAromaSHA1(AromaAPI, Inkay::Versions::WMS::JuxtSHA1, Inkay::Versions::WPS::JuxtSHA1);
            Inkay::JSON::ParseAromaSHA1(AromaAPI, Inkay::Versions::WMS::JuxtSHA1, Inkay::Versions::WPS::JuxtSHA1);

            bool ok = Inkay::Versions::VerifyJuxtInstall();

            SYS::Report::Log("\n-----------------------------------\n");
            SYS::Report::Log("Inkay Info\n");
            SYS::Report::Log("-----------------------------------\n");
            SYS::Report::Log("Local WMS: %s\n", Inkay::Versions::WMS::LocalSHA1.c_str());
            SYS::Report::Log("Local WPS: %s\n", Inkay::Versions::WPS::LocalSHA1.c_str());
            SYS::Report::Log("Juxt WMS: %s\n", Inkay::Versions::WMS::JuxtSHA1.c_str());
            SYS::Report::Log("Juxt WPS: %s\n", Inkay::Versions::WPS::JuxtSHA1.c_str());
            SYS::Report::Log("Juxt PLG: %s\n\n", ok ? "true" : "false");
            SYS::Report::Log("Juxt URL: %s\n", Web::JuxtFileURL.c_str());
            SYS::Report::Log("Rose URL: %s\n", Web::RoseFileURL.c_str());
            SYS::Report::Log("-----------------------------------\n\n");
        }

        void AboutBrowserURL(void) { SYS::Apps::BrowserOpenViewer(BrowserURL); }

        void Shutdown(void) {
            if (!Web::JuxtFileURL.empty()) Web::JuxtFileURL.clear();
            if (!Web::RoseFileURL.empty()) Web::RoseFileURL.clear();

            if (!Inkay::Versions::WMS::LocalSHA1.empty()) Inkay::Versions::WMS::LocalSHA1.clear();
            if (!Inkay::Versions::WMS::JuxtSHA1.empty()) Inkay::Versions::WMS::JuxtSHA1.clear();
            if (!Inkay::Versions::WPS::LocalSHA1.empty()) Inkay::Versions::WPS::LocalSHA1.clear();
            if (!Inkay::Versions::WPS::JuxtSHA1.empty()) Inkay::Versions::WPS::JuxtSHA1.clear();
        }
    }
}