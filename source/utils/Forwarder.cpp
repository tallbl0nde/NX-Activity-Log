#include <filesystem>
#include "utils/Forwarder.hpp"
#include "utils/Utils.hpp"

// Variables used for installation/detection
// Realistically I could do without them but this saves repeat 'exists' calls
static bool atms;
static bool rei;
static bool sx;
static bool hasAtms;
static bool hasRei;
static bool hasSx;

namespace Utils::Forwarder {
    void initVars() {
        atms = false;
        rei = false;
        sx = false;
        hasAtms = false;
        hasRei = false;
        hasSx = false;

        // Check for files
        if (std::filesystem::exists("/atmosphere/contents")) {
            hasAtms = true;
            if (std::filesystem::exists("/atmosphere/contents/0100000000001013/exefs.nsp")) {
                atms = true;
            }
        }
        if (std::filesystem::exists("/ReiNX/contents")) {
            hasRei = true;
            if (std::filesystem::exists("/ReiNX/contents/0100000000001013/exefs.nsp")) {
                rei = true;
            }
        }
        if (std::filesystem::exists("/sxos/titles")) {
            hasSx = true;
            if (std::filesystem::exists("/sxos/titles/0100000000001013/exefs.nsp")) {
                sx = true;
            }
        }
    }

    bool installed() {
        return (atmosphere() || reinx() || sxos());
    }

    void uninstall() {
        if (atms) {
            std::filesystem::remove("/atmosphere/contents/0100000000001013/exefs.nsp");
            atms = false;
        }
        if (rei) {
            std::filesystem::remove("/ReiNX/contents/0100000000001013/exefs.nsp");
            rei = false;
        }
        if (sx) {
            std::filesystem::remove("/sxos/titles/0100000000001013/exefs.nsp");
            sx = false;
        }
    }

    void install() {
        if (hasAtms) {
            std::filesystem::create_directory("/atmosphere/contents/0100000000001013");
            Utils::copyFile("romfs:/exefs.nsp", "/atmosphere/contents/0100000000001013/exefs.nsp");
            atms = true;
        }

        if (hasRei) {
            std::filesystem::create_directory("/ReiNX/contents/0100000000001013");
            Utils::copyFile("romfs:/exefs.nsp", "/ReiNX/contents/0100000000001013/exefs.nsp");
            rei = true;
        }

        if (hasSx) {
            std::filesystem::create_directory("/sxos/titles/0100000000001013");
            Utils::copyFile("romfs:/exefs.nsp", "/sxos/titles/0100000000001013/exefs.nsp");
            sx = true;
        }
    }

    bool prepared() {
        return std::filesystem::exists("/switch/NX-Activity-Log/NX-Activity-Log.nro");
    }

    bool atmosphere() {
        return atms;
    }

    bool reinx() {
        return rei;
    }

    bool sxos() {
        return sx;
    }
};