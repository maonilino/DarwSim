#define GLEW_STATIC
#include "Environment.h"

int main(int argc, char const* argv[])
{
    void* simHandler = dlopen("./src/libDarwSimSim.so", RTLD_LAZY);
    if (!simHandler) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // \copydoc Environment::create_environment
    Environment* (*createEnv)(std::vector<std::string> & arguments);
    std::unique_ptr<Environment> env;

    std::vector<std::string> options;

    // options.emplace_back(argv[0]);

    // skip the path of the bin, only keep passed args
    for (int i = 1; i < argc; ++i)
        options.emplace_back(argv[i]);

#ifdef unix
    createEnv = (Environment * (*)(std::vector<std::string> & arguments))
        dlsym(simHandler, "create_environment");
    if (createEnv) { // check whether funuction loaded from shared lib properly
        env.reset(createEnv(options));
        env->runSimulation();
    }
#endif
#ifdef _WIN32
// load function from shared library
#endif
    else
        std::cerr << "green environment not yet fully supported.\n";

    // -----------------------------------------------------------------------------------------

    return 0;
}