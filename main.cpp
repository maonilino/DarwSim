#define GLEW_STATIC
#include "Environment.h"

int main(int argc, char** argv)
{
    void* simHandler = dlopen("./src/libDarwSimSim.so", RTLD_LAZY);
    if (!simHandler) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // \copydoc Environment::create_environment
    Environment* (*createEnv)(int, char**);
    std::unique_ptr<Environment> env;

#ifdef unix
    createEnv = (Environment * (*)(int, char**))
        dlsym(simHandler, "create_environment");
    if (createEnv) { // check whether funuction loaded from shared lib properly
        try
        {
            env.reset(createEnv(argc, argv));
            env->runSimulation();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
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