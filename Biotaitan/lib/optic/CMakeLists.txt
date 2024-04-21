idf_component_register(SRCS "optic.c"
                            "../ADS1115/ADS1115.c"
                            "../i2c_api/i2c_api.c"
                       INCLUDE_DIRS "."
                                    "../ADS1115"
                                    "../i2c_api"
                       REQUIRES "driver" "esp_timer"
)
