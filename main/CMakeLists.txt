idf_component_register(SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES slint)

slint_target_sources(${COMPONENT_LIB} ui/app-window.slint)
add_custom_target(invalidate_files ALL touch ../../../main/ui/app-window.slint)
