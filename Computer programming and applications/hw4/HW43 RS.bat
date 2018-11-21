g++ HW41.cpp -o HW41.exe

for /L %%i in (1, 1, 2500) do (
    echo ./data/data_7_14_%%i.txt 0 > command.txt
    HW41.exe < command.txt >> output_file1.txt
    echo HW41 - %%i
)
del command.txt
for /L %%i in (1, 1, 2500) do (
    echo ./data/data_8_13_%%i.txt 0 > command.txt
    HW41.exe < command.txt >> output_file1.txt
    echo HW41 - %%i
)
del command.txt
for /L %%i in (1, 1, 2500) do (
    echo ./data/data_10_10_%%i.txt 0 > command.txt
    HW41.exe < command.txt >> output_file1.txt
    echo HW41 - %%i
)
del command.txt
for /L %%i in (1, 1, 2500) do (
    echo ./data/data_12_12_%%i.txt 0 > command.txt
    HW41.exe < command.txt >> output_file1.txt
    echo HW41 - %%i
)
del command.txt