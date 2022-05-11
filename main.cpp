#define GLEW_STATIC
#include "Environment.h"
#include <map>

int main(int argc, char const* argv[])
{
    void* simHandler = dlopen("./src/libDarwSimSim.so", RTLD_LAZY);
    if (!simHandler) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    Environment* (*createEnv)();
    std::unique_ptr<Environment> env;

    std::map<int, std::string> options;

    // skip the path of the bin, only keep passed args
    for (int i = 1; i < argc; ++i)
        options[i] = argv[i];

#ifdef unix
    createEnv =
        (Environment * (*)()) dlsym(simHandler, "create_environment");
    if (createEnv) { // check whether funuction loaded from shared lib properly
        env.reset(createEnv());
        env->runSimulation();
    }
#endif
#ifdef _WIN32
// load function from shared library
#endif
    else
        std::cerr << "green environment not yet fully supported.\n";

    // -----------------------------------------------------------------------------------------

    // env->setArguments(options);

    return 0;
}