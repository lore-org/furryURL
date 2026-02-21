#pragma once

namespace furryurl {
    enum HTTPMethod : int {
        Get,
        Head,
        Post,
        Put,
        Delete,
        Connect,
        Options,
        Trace,
        Patch
    };

    using Method = HTTPMethod;
}