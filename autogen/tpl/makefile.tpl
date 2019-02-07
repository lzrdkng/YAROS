[< AutoGen5 template -*- mode:makefile -*- makefile >]
[< (out-move "makefile") >][< HEADER >]
AUTO_GENERATED=[< FOR auto_generate " " >][< TARGET >][< ENDFOR auto_gen >]

[< FOR variables >]
[< name >]=[< value >]
[< ENDFOR variables >]

INC_DIR=[< inc_dirs >]

BUILD_PATH=[< FOR build_dirs " " >][< build_dirs >][< ENDFOR build_dirs >]

SRC=[< FOR src_files " " >][< src_files >][< ENDFOR src_files >]

ARCH_SRC=[< FOR arch " " >][< arch >][< ENDFOR arch >]

SRC += $(ARCH_SRC)

MCU=[< (getenv "MCU") >]

[< (insert-file "Makefile.in") >]

[< FOR auto_generate "" >]
[< TARGET >]: [< definition >] [< template >]
	@autogen --select-suffix=[< suffix >] --override-tpl=[< template >] --definitions=[< definition >] --base-name=[< base_name >]
	@mv [< base_name >].[< suffix >] [< dir >]
	@echo "AUTOGEN" [< TARGET >]
[< ENDFOR auto_generate >]

[< define HEADER >]
[< (license-full "gpl" "YAROS" "# " "Olivier Dion" "2018") >]

[< (dne "-D" "# ") >]
[< enddef >]

[< define TARGET >][< (join "" (get "dir") (get "base_name") "." (get "suffix")) >][< enddef >]
