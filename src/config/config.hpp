// Contains a configuration module for the application.
// Soultatos Stefanos 2022

#ifndef CONFIG_CONFIG_HPP
#define CONFIG_CONFIG_HPP

// Supress warnings, intellisense errors.
#ifndef ARCHV_CONFIG_DIR
#define ARCHV_CONFIG_DIR
#endif

// The path of the dependencies.json config file.
#define ARCHV_DEPS_CONFIG_PATH ARCHV_CONFIG_DIR "/dependencies.json"

// The path of the layout.json config file.
#define ARCHV_LAYOUT_CONFIG_PATH ARCHV_CONFIG_DIR "/layout.json"

// The path of the rendering.json config file.
#define ARCHV_RENDERING_CONFIG_PATH ARCHV_CONFIG_DIR "/rendering.json"

#endif // CONFIG_CONFIG_HPP
