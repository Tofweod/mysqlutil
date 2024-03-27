set_toolset("cc", "gcc")
set_toolset("sh", "gcc")
set_toolset("ld", "gcc")

target("mysqlutil")
do
	set_targetdir("$(projectdir)/so")
	set_kind("shared")
	add_files("*.c")

	add_syslinks("mysqlclient")
	add_syslinks("lua")

	includes("test")
end
