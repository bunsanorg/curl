#pragma once

#include <bunsan/curl/easy.hpp>

#include <string>

struct easy_fixture
{
    bunsan::curl::easy easy;
    const std::string url_root = "http://localhost:8090";
    std::string data;

    std::function<std::size_t (char *, std::size_t)> data_writer =
        [this](char *const ptr, const std::size_t size)
        {
            data.append(ptr, size);
            return size;
        };
};
