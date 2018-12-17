# xxtea_packer

使用xxtea加密文件或脚本时，发现工具都是python的，依赖比较多，因此自己写了一个c++工具，没有依赖工具仅仅31k。

命令列表:
1、encode : use xxtea_key encode src_dir's files, output in dst_dir!
  format: encode  src_dir|string  dst_dir|string  xxtea_key|string  signment|string
2、decode : use xxtea_key decode src_dir's files, output in dst_dir!
  format: decode  src_dir|string  dst_dir|string  xxtea_key|string  signment|string
  example : encode ../cbin/src/ ./encode/ cogame XXTEA;
  example : decode . /encode  . /decode/ cogame XXTEA

批处理加上工具文件名即可。
example : xxtea_packer encode ../cbin/src/ ./encode/ cogame XXTEA 

注意： 文件目录必须有斜杠'\\'!
