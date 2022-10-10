#!/usr/bin/env bash

# aes blowfish des 3des rc2 rc5
algorithms="aes blowfish des 3des rc2 rc5"

models="cbc cfb ctr ofb" # 加密模式
for i in $(seq 1 10); do block_sizes="${block_sizes} $((2**$i))"; done # 单位：KB
key_size="512" # 密钥长度

BUILD_DIR="build"
PROGRAM=${BUILD_DIR}/CMP
RESULT_DIR=generate/$(date "+%F_%H%M")

# 准备工作
cmake -B $BUILD_DIR
make -C $BUILD_DIR
mkdir -p $RESULT_DIR

# 重复$1次获取加解密平均值，输出：加密时间，解密时间
function get_time()
{
    for i in $(seq $1); do
        $PROGRAM $2 $3 $4 $5
    done |awk '{ encrypt_time += $5; decrypt_time += $6 }
            END {print int(encrypt_time / NR), int(decrypt_time / NR)}'
}

# 生成数据
for algorithm in $algorithms; do
    data_file="$RESULT_DIR/$algorithm.txt" # 包含加解密时间的数据文件
    plot_data_file="$RESULT_DIR/${algorithm}_plot.txt" # 用于画图的数据文件
    echo "正在生成${algorithm}数据文件"

    echo -n "blocksize" > $data_file  # -n means no newline; the function of this command is to write "blocksize" to the file without newline at the end
    for model in $models; do echo -n " ${model}_encrypt ${model}_decrypt" >> $data_file; done
    echo >> $data_file
    # echo "blocksize $models" > $plot_data_file
    echo "blocksize" > $plot_data_file
    # captialize the letters in $models
    captial_models=${models^^}
    echo "blocksize $captial_models" > $plot_data_file
    for size in $block_sizes; do
        echo -n "$size" >> $data_file
        echo -n "$size" >> $plot_data_file
        for model in $models; do
            time=$(get_time 1 $algorithm $model $key_size $((size*1024)))
            echo -n " $time" >> $data_file
            echo $time |awk '{printf " " $1+$2}' >> $plot_data_file
        done
        echo >> $data_file
        echo >> $plot_data_file
    done
done

# 画图 1
for algorithm in $algorithms; do
    data_file="$RESULT_DIR/${algorithm}_plot.txt"
    output_file="$RESULT_DIR/$algorithm.pdf"
    script_file=$(mktemp)
cat <<EOF >$script_file
set term pdfcairo; set output "$output_file"
# set title "$algorithm"
set style data linespoints
set key left
set xlabel 'Block size (KB)'
set ylabel 'Time (microseconds)'
plot "$data_file" using 2:xtic(1) title columnheader(2), for [i=3:5] '' using i title columnheader(i)
EOF

    gnuplot $script_file
done

# 生成数据
for size in $block_sizes; do
    plot_data_file="$RESULT_DIR/compare_$size.txt"
    output_file="$RESULT_DIR/compare_$size.pdf"

    echo "algorithm Encryption Decryption" > $plot_data_file
    for algorithm in $algorithms; do
        input_file="$RESULT_DIR/${algorithm}.txt"
        script_file=$(mktemp)
cat <<EOF >$script_file
NR == 1 {split(\$0, header)}
\$1 == $size {
    min = 2;
    for (i = 4; i < NF; i+=2)
        min = (\$min + \$(min+1) > \$i + \$(i+1)) ? i : min;
    print "(" toupper(substr(header[min], 0, 3)) ")", \$min, \$(min+1);
}
EOF

        # echo -n "${algorithm^^}" >> $plot_data_file
        if test $algorithm == "blowfish"; then
            echo -n "Blowfish" >> $plot_data_file
        else
            echo -n "${algorithm^^}" >> $plot_data_file
        fi
        cat $input_file |awk -f $script_file >> $plot_data_file
    done

    # 画图 2
    script_file=$(mktemp)
cat <<EOF >$script_file
set term pdfcairo; set output "$output_file"
# set title "${size}KB"
set ylabel 'Time (microseconds)'
set xlabel '${size}KB'
set key autotitle columnheader
set style data histogram
set style histogram rowstacked
set style fill solid border -1
set xtics rotate by -45
set boxwidth 0.75
plot "$plot_data_file" using 2:xticlabels(1), '' using 3
EOF

    gnuplot $script_file
done
