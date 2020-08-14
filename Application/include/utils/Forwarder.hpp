#ifndef UTILS_FORWARDER_HPP
#define UTILS_FORWARDER_HPP

// These are a bunch of helper functions used to detect if the forwarder
// is installed, etc...
namespace Utils::Forwarder {
    // Inits variables (don't need to cleanup - just reinit!)
    void initVars();

    // Is it installed?
    bool installed();

    // Uninstall (delete file from relevant folders)
    void uninstall();

    // Install (copy file to relevant folders)
    void install();

    // Is the .nro in the right location?
    bool prepared();

    // Returns true if installed for CFW
    bool atmosphere();
    bool reinx();
    bool sxos();
};

#endif