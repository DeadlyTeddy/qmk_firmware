# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes        # Console for debug
COMMAND_ENABLE = yes        # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
RGB_MATRIX_ENABLE = yes     # Enable per-key RGB matrix
RGB_MATRIX_DRIVER = ws2812  # Use WS2812 driver for RGB matrix
WS2812_DRIVER = pwm         # Use PWM+DMA for reliable WS2812 timing on STM32
AUDIO_ENABLE = no           # Audio output
SWAP_HANDS_ENABLE = no      # Temporarily disabled
OLED_ENABLE = yes            # OLED display

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3389
MOUSE_SHARED_EP = yes

COMBO_ENABLE = yes

WPM_ENABLE = yes

SERIAL_DRIVER = usart

# DEFAULT_FOLDER = handwired/tractyl_manuform/5x6_right/teensy2pp
