set_toolset("cc", "gcc")
set_toolset("sh", "gcc")
set_toolset("ld", "gcc")

target("mysqlutil")
do
	set_targetdir("$(projectdir)/so")
	set_kind("shared")
	add_files("*.c")

	add_includedirs("include")
	add_cxflags("-x c")

	add_syslinks("mysqlclient")
	add_syslinks("lua")

	after_build(function(target)
		os.cp("lua/", target:targetdir())
		os.cp("include/mysql_util.h", target:targetdir())
	end)
end
