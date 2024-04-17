idf_component_register(SRCS "optic.c" "../ADS1115/ADS1115.c"
                       INCLUDE_DIRS "." "../ADS1115"
                       REQUIRES "driver" "esp_timer"
)
