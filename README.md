FFmpeg 提供了大量的滤镜，涵盖了视频和音频的各种处理需求。以下是一些常见的滤镜及其功能的概述：

视频滤镜
crop: 裁剪视频的某个区域。
scale: 缩放视频。
transpose: 旋转和翻转视频。
rotate: 根据指定的角度旋转视频。
hflip: 水平翻转视频。
vflip: 垂直翻转视频。
drawtext: 在视频上绘制文本。
overlay: 叠加一个视频在另一个视频上。
fade: 淡入淡出效果。
drawbox: 在视频上绘制矩形框。
geq: 使用表达式处理每个像素。
lenscorrection: 校正镜头畸变。
noise: 添加噪声到视频。
null: 不做任何处理。
removelogo: 移除视频中的标志。
pad: 补齐视频边缘。
setpts: 修改时间戳。
split: 将输入拆分成多个输出。
tpad: 向视频前或后添加空白帧。
curves: 调整视频颜色曲线。
chromakey: 颜色键抠像。
colormatrix: 颜色矩阵转换。
lut: 使用查找表处理每个像素。
lut3d: 3D查找表。
deshake: 视频防抖。
select: 根据表达式选择帧。
zoompan: 缩放和平移效果。
colorbalance: 颜色平衡调整。
colorkey: 基于颜色键抠像。
eq: 调整亮度、对比度、饱和度和伽玛。
frei0r: 使用 frei0r 插件。
hue: 调整色相和饱和度。
lutrgb: 使用 RGB 查找表。
lutyuv: 使用 YUV 查找表。
mpdecimate: 去除重复帧。
negate: 反转视频颜色。
rotate: 旋转视频。
setdar: 设置显示宽高比。
setsar: 设置采样宽高比。
sidedata: 管理视频元数据。
threshold: 阈值处理。
tmix: 时间混合效果。
unsharp: 锐化滤镜。
uspp: 去噪。
yadif: 去交错。
音频滤镜
volume: 调整音量。
aecho: 添加回声效果。
anull: 不做任何处理。
areverse: 反转音频。
asetrate: 设置采样率。
atempo: 调整播放速度。
compand: 动态范围压缩。
pan: 调整声道。
silenceremove: 移除静音部分。
treble: 高音增强。
bass: 低音增强。
aformat: 转换音频格式。
alimiter: 限幅器。
highpass: 高通滤波。
lowpass: 低通滤波。
equalizer: 均衡器。
silencedetect: 检测静音部分。
dynaudnorm: 动态音频标准化。
chorus: 添加合唱效果。
afftdn: FFT 去噪。
afftfilt: FFT 滤波。
afade: 淡入淡出。
agate: 噪声门。
ahighpass: 高通滤波。
alowpass: 低通滤波。
allpass: 全通滤波。
bandpass: 带通滤波。
bandreject: 带阻滤波。
treble: 高音增强。
bass: 低音增强。
replaygain: 调整音量以实现回放增益。
silenceremove: 移除静音部分。
sidechaincompress: 侧链压缩。
stereotools: 立体声工具。
vibrato: 添加颤音效果。
volumedetect: 检测音量。