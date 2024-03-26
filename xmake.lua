target("mysqlutil")
do
	set_targetdir("$(projectdir)/so")
	set_kind("shared")
	add_files("util.c")
	add_syslinks("mysqlclient")

	includes("test")
end
