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
AUDIO_ENABLE = no           # Audio output
SWAP_HANDS_ENABLE = no      # Temporarily disabled
OLED_ENABLE = no            # Temporarily disabled to test split communication

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3389
MOUSE_SHARED_EP = yes

COMBO_ENABLE = yes

SERIAL_DRIVER = usart

# DEFAULT_FOLDER = handwired/tractyl_manuform/5x6_right/teensy2pp
