#include <asynclog/logger.h>

int main()
{
    asynclog::Logger::initialize();
    asynclog::info("Starting", "main()");
    asynclog::info("Hello World!", "main()");
    asynclog::info("Finishing", "main()");

    for (std::size_t i = 0; i < 100; ++i)
        asynclog::debug(std::to_string(i), "main()");

    return 0;
}
