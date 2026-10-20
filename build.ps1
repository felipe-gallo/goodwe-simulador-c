$compiler = Get-Command clang, gcc -ErrorAction SilentlyContinue | Select-Object -First 1

if ($null -eq $compiler) {
    Write-Host "Nenhum compilador C foi encontrado. Instale LLVM/Clang ou MinGW-w64 e abra um novo terminal."
    exit 1
}

& $compiler.Source -Wall -Wextra -Wpedantic -std=c11 main.c sessao.c -o goodwe_simulador.exe
if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilacao concluida: .\\goodwe_simulador.exe"
}
