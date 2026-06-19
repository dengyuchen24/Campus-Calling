# CMakeList.txt: CampusCalling 的 CMake 项目配置
# 最低 CMake 版本要求（SFML 3.0 建议 3.21+）
cmake_minimum_required (VERSION 3.21)

# 项目名称 + C++ 标准
project (CampusCalling LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------- 配置 SFML 3.0 --------------------------
# SFML 安装根目录
set(SFML_ROOT "D:/Code/C++/SFML")

# 根据编译类型（Debug/Release）选择对应的 SFML 库后缀
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(SFML_SUFFIX "-d")
else()
	set(SFML_SUFFIX "")
endif()

# -------------------------- 项目源代码 --------------------------
add_executable (CampusCalling 
	"CampusCalling.cpp" 
	"CampusCalling.h" 
	"dycLogger/Logger.cpp"
	"dycLogger/Logger.h"
	"WndManager/sfWindow.cpp"
	"WndManager/sfWindow.h"
	"Characters/character.cpp"
	"Characters/character.h"
	"Effects/src/Effect.hpp"
	"Effects/src/EffectTarget.hpp"
	"Effects/src/Trigger.hpp"
	"Effects/EffectSystem.hpp"
	"WndManager/wndManager/GameWnd/GameWnd.h"
	"WndManager/wndManager/GameWnd/GameWnd.cpp"
	"Cards/cards.cpp"
	"Cards/cards.h"
	"Characters/characters/CaiShuYou/CaiShuYou.h"
	"Characters/characters/CaiShuYou/CaiShuYou.cpp"
	"WndManager/wndManager/PreparationWnd/Preparation.cpp"
	"WndManager/wndManager/PreparationWnd/Preparation.h"
	"WndManager/wndManager/PreparationWnd/StartGameButton.h"
	"WndManager/wndManager/PreparationWnd/StartGameButton.cpp"
)

# -------------------------- 包含目录--------------------------
# 使用 target_include_directories 为当前目标设置头文件路径
# 这里直接使用 SFML_ROOT 拼接路径，不再依赖未定义的变量
target_include_directories(CampusCalling PRIVATE
	${SFML_ROOT}/include
	# 如果你的项目有其他头文件目录，也加在这里
)

# -------------------------- 链接 SFML 库 --------------------------
# 链接库目录
target_link_directories(CampusCalling PRIVATE
	${SFML_ROOT}/lib
)

# SFML 3.0 核心模块（根据你的需求添加/删减）
target_link_libraries(CampusCalling PRIVATE
	sfml-system${SFML_SUFFIX}
	sfml-window${SFML_SUFFIX}
	sfml-graphics${SFML_SUFFIX}
	sfml-audio${SFML_SUFFIX}
	sfml-network${SFML_SUFFIX}
)

# 可选：将 SFML 的 dll 复制到输出目录（Windows 必备）
if (WIN32)
	# 复制 Release 版本 dll
	file(GLOB SFML_RELEASE_DLLS "${SFML_ROOT}/bin/*.dll")
	list(FILTER SFML_RELEASE_DLLS EXCLUDE REGEX "-d.dll$")
	foreach(DLL ${SFML_RELEASE_DLLS})
		add_custom_command(TARGET CampusCalling POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${DLL} $<TARGET_FILE_DIR:CampusCalling>
		)
	endforeach()
endif()

# 适配 CMake 版本的 CXX_STANDARD 属性（原有逻辑保留）
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CampusCalling PROPERTY CXX_STANDARD 20)
endif()