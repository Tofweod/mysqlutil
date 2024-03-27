target("test")
do
	set_kind("binary")
	add_files("test.c")

	add_defines("DEBUG")

	add_deps("mysqlutil")
	add_syslinks("mysqlclient")

	after_build(function(target)
		os.cp("$(projectdir)/test/config.lua", target:targetdir())
	end)
end
