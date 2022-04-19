#include "Environment.h"
#include <map>

int main(int argc, char const* argv[])
{
    void* simHandler = dlopen("./src/libDarwSimSim.so", RTLD_LAZY);
    /**
     * @todo pass the colour of the env inside this function (for the constructor?).
     * Change pointer type
     *
     */
    void (*deleteEnvironment)(Environment*);
    Environment* (*createGreenEnv)();
    std::unique_ptr<Environment> (*createBrownEnv)();
    std::unique_ptr<Environment> (*createYellowEnv)();

    deleteEnvironment =
        (void (*)(Environment*))dlsym(simHandler, "destroy_environment");

    std::map<int, std::string> options;

    // skip the path of the bin, only keep passed args
    for (int i = 1; i < argc; ++i)
        options[i] = argv[i];

    Environment* env;

#ifdef unix
    createGreenEnv =
        (Environment * (*)()) dlsym(simHandler, "create_green_environment");
    if (createGreenEnv) { // check whether funuction loaded from shared lib properly
        env = (Environment*)createGreenEnv();
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
    deleteEnvironment(env);

    return 0;
}