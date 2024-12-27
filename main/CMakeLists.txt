idf_component_register(SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES slint esp32_io_expander esp_lcd_touch_gt911)

slint_target_sources(${COMPONENT_LIB} ui/app-window.slint)

# ui wont build changes from files other than app-window.slint, unless app-window.slint is modified and rebuilt
# this forces it to rebuild
add_custom_target(invalidate_files ALL touch ../../../main/ui/app-window.slint)
