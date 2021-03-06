#include "shared.hh"
#include "store-api.hh"

using namespace nix;

int main(int argc, char ** argv)
{
    return handleExceptions(argv[0], [&]() {
        initNix();

        auto gzip = false;
        auto toMode = true;
        auto includeOutputs = false;
        auto dryRun = false;
        auto useSubstitutes = false;
        std::string sshHost;
        PathSet storePaths;

        parseCmdLine(argc, argv, [&](Strings::iterator & arg, const Strings::iterator & end) {
            if (*arg == "--help")
                showManPage("nix-copy-closure");
            else if (*arg == "--version")
                printVersion("nix-copy-closure");
            else if (*arg == "--gzip" || *arg == "--bzip2" || *arg == "--xz") {
                if (*arg != "--gzip")
                    printMsg(lvlError, format("Warning: ‘%1%’ is not implemented, falling back to gzip") % *arg);
                gzip = true;
            } else if (*arg == "--from")
                toMode = false;
            else if (*arg == "--to")
                toMode = true;
            else if (*arg == "--include-outputs")
                includeOutputs = true;
            else if (*arg == "--show-progress")
                printMsg(lvlError, "Warning: ‘--show-progress’ is not implemented");
            else if (*arg == "--dry-run")
                dryRun = true;
            else if (*arg == "--use-substitutes" || *arg == "-s")
                useSubstitutes = true;
            else if (sshHost.empty())
                sshHost = *arg;
            else
                storePaths.insert(*arg);
            return true;
        });

        if (sshHost.empty())
            throw UsageError("no host name specified");

        auto remoteUri = "legacy-ssh://" + sshHost + (gzip ? "?compress=true" : "");
        auto to = toMode ? openStore(remoteUri) : openStore();
        auto from = toMode ? openStore() : openStore(remoteUri);

        PathSet closure;
        from->computeFSClosure(storePaths, closure, false, includeOutputs);

        copyPaths(from, to, Paths(closure.begin(), closure.end()), useSubstitutes);
    });
}
