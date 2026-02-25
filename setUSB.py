Import("env")
board_config = env.BoardConfig()
#board_config.update("build.hwids", [["0x256f", "0xC635"]])  #Methode 1
board_config.update("upload_port.0.vid", 0x2341)   # Methode 2
board_config.update("upload_port.0.pid", 0x8037)   # Methode 2
board_config.update("build.usb_product", "ORBJOY Left")
board_config.update("vendor", "ORBION")

#Original
#"USB_VID=0x2341",
#"USB_PID=0x8037",