$path_splitter = $([System.IO.Path]::DirectorySeparatorChar)
$no_path_splitter = ''
$no_escape_splitter = '\\'
if ($path_splitter -eq '\') {
	$no_path_splitter = '/'
} else {
	$no_path_splitter = '\'
}

$buildignore_path = (Get-Location).Path + $path_splitter + '.buildignore'

[String[]]$buildignore_targets = [System.IO.File]::ReadAllLines($buildignore_path)

$current_location = $(Get-Location).Path + $path_splitter

foreach ($i in 0..($buildignore_targets.Count - 1)) {
	$buildignore_targets[$i] = ($current_location + $buildignore_targets[$i]).Replace($no_escape_splitter, $path_splitter).Replace($no_path_splitter, $path_splitter)
}

(Get-ChildItem -r) | ForEach-Object {
	[System.IO.FileSystemInfo]$info = $_
	if (-Not $buildignore_targets.Contains($info.FullName)) {
		if ($info.Name.EndsWith('.hpp')) {
			echo "Compiling: $($info.Name)"
			clang "$($info.FullName)" -S -Wall -Wpedantic -Wextra -Weffc++ -Werror -std=c++20 -Wfatal-errors -fsyntax-only -Og
			if ($LASTEXITCODE -ne 0) {
				break
			}
		}
	}
}