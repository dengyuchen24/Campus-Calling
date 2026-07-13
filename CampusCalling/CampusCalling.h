// CampusCalling.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

// 在此处引用程序需要的标头。

#include "WndManager/sfWindow.h"

namespace dyc { using json = nlohmann::json; }

template<typename T>
std::map<std::string, T> parseArrayToMap(const nlohmann::json& arr) {
    std::map<std::string, T> result;
    for (const auto& item : arr) {
        T obj = item.get<T>();          // 自动调用 NLOHMANN 生成的 from_json
        result[obj.name] = std::move(obj);
    }
    return result;
}