idf_component_register(SRCS "barcode.c"
                       INCLUDE_DIRS "."
                                    "../barcode"
                       REQUIRES "driver" "esp_timer"
)
