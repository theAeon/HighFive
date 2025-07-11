/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#pragma once

#include <exception>
#include <string>
#include <vector>

#include <H5Ipublic.h>

namespace HighFive {

namespace details {

// iterator for H5 iterate

struct HighFiveIterateData {
    explicit HighFiveIterateData(std::vector<std::string>& my_names)
        : names(my_names)
        , err(nullptr) {}

    std::vector<std::string>& names;
    std::exception* err;

    inline void throwIfError() const {
        if (err != nullptr) {
            throw *err;
        }
    }
};

template <typename InfoType>
inline herr_t internal_high_five_iterate(hid_t /*id*/,
                                         const char* name,
                                         const InfoType* /*info*/,
                                         void* op_data) {
    auto* data = static_cast<HighFiveIterateData*>(op_data);
    try {
        data->names.emplace_back(name);
        return 0;
    } catch (...) {
        data->err = new ObjectException("Exception during H5Iterate, abort listing");
    }
    return -1;
}

}  // namespace details
}  // namespace HighFive
