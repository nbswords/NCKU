g++ HW42.cpp -o HW42.exe

for /L %%i in (1, 1, 2500) do (
    echo ./data/data_7_14_%%i.txt 0 > command.txt
    HW42.exe < command.txt >> output_file2.txt
    echo HW42 - %%i
)
del command.txt
for /L %%i in (1, 1, 2500) do (
    echo ./data/data_8_13_%%i.txt 0 > command.txt
    HW42.exe < command.txt >> output_file2.txt
    echo HW42 - %%i
)
del command.txt
for /L %%i in (1, 1, 2500) do (
    echo ./data/data_10_10_%%i.txt 0 > command.txt
    HW42.exe < command.txt >> output_file2.txt
    echo HW42 - %%i
)
del command.txt
for /L %%i in (1, 1, 2500) do (
    echo ./data/data_12_12_%%i.txt 0 > command.txt
    HW42.exe < command.txt >> output_file2.txt
    echo HW42 - %%i
)
del command.txt