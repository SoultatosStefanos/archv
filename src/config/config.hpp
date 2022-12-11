// Contains a configuration module for the application.
// Soultatos Stefanos 2022

#ifndef CONFIG_CONFIG_HPP
#define CONFIG_CONFIG_HPP

// The path of the weights.json config file.
#ifndef ARCHV_WEIGHTS_CONFIG_PATH
#error "Archv weights config path not found!"
#endif

// The path of the layout.json config file.
#ifndef ARCHV_LAYOUT_CONFIG_PATH
#error "Archv layout config path not found!"
#endif

// The path of the rendering.json config file.
#ifndef ARCHV_RENDERING_CONFIG_PATH
#error "Archv rendering config path not found!"
#endif

// The path of the gui.json config file.
#ifndef ARCHV_GUI_CONFIG_PATH
#error "Archv gui config path not found!"
#endif

// The path of the scaling.json config file.
#ifndef ARCHV_SCALING_CONFIG_PATH
#error "Archv scaling config path not found!"
#endif

// The path of the clustering.json config file.
#ifndef ARCHV_CLUSTERING_CONFIG_PATH
#error "Archv clustering config path not found!"
#endif

#ifndef ARCHV_RESOURCE_GROUP
#error "Archv resource group not found!"
#endif

#endif // CONFIG_CONFIG_HPP
