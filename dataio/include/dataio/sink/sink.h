#ifndef DATAIO_SINK_H
#define DATAIO_SINK_H

#include <memory>
#include <string>

namespace dataio {

    class Sink {
    public:
        virtual ~Sink() = default;
        virtual bool write(std::string message) = 0;
    };

}

#endif //DATAIO_SINK_H