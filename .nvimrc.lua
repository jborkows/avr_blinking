function GenerateCompileCommandsAndRestart()
	vim.cmd("!bear -- make")
	print("Generated compile_commands.json. Restarting Neovim...")
	vim.cmd("edit")
end

vim.api.nvim_create_autocmd("BufWritePost", {
	pattern = "Makefile",
	callback = function()
		GenerateCompileCommandsAndRestart()
	end,
})

local lspconfig = require("lspconfig")
print("Setting up clangd for AVR")

lspconfig.clangd.setup({
	root_dir = lspconfig.util.root_pattern("compile_commands.json", "Makefile", ".git"),
	cmd = { "clangd" },
	filetypes = { "c", "cpp" },
})
