add_rules("mode.debug", "mode.release")

target("qsh")
    set_kind("binary")
    add_files("*.c")
