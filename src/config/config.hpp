// Contains a configuration module for the application.
// Soultatos Stefanos 2022

#ifndef CONFIG_CONFIG_HPP
#define CONFIG_CONFIG_HPP

// Supress warnings, intellisense errors.
#ifndef ARCHV_CONFIG_DIR
#define ARCHV_CONFIG_DIR
#endif

// The path of the weights.json config file.
#define ARCHV_WEIGHTS_CONFIG_PATH ARCHV_CONFIG_DIR "/weights.json"

// The path of the layout.json config file.
#define ARCHV_LAYOUT_CONFIG_PATH ARCHV_CONFIG_DIR "/layout.json"

// The path of the rendering.json config file.
#define ARCHV_RENDERING_CONFIG_PATH ARCHV_CONFIG_DIR "/rendering.json"

// The path of the gui.json config file.
#define ARCHV_GUI_CONFIG_PATH ARCHV_CONFIG_DIR "/gui.json"

// The path of the scaling.json config file.
#define ARCHV_SCALING_CONFIG_PATH ARCHV_CONFIG_DIR "/scaling.json"

// The path of the clustering.json config file.
#define ARCHV_CLUSTERING_CONFIG_PATH ARCHV_CONFIG_DIR "/clustering.json"

#ifndef ARCHV_RESOURCE_GROUP
#define ARCHV_RESOURCE_GROUP "Archv"
#endif

#endif // CONFIG_CONFIG_HPP
