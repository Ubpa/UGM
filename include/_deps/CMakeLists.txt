if("${Ubpa_USE_XSIMD}" STREQUAL "ON")
	Ubpa_DownloadZip(
		https://ubpa.github.io/UData/UGM/xsimd.zip
		xsimd.zip
		SHA256 77EFEE86C767C2DE08D59160FF0B686155DAF558BB3FDD944A9D4E290B499BC6
	)
endif()
