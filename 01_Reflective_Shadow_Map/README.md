# 项目简介
* 本项目为对部分图形学论文的复现，主要代码框架和思路参考了AngelMonica126的图形学论文复现的开源项目https://github.com/AngelMonica126/GraphicAlgorithm ，在其基础上加入了自己的理解，并对部分公式参考原论文进行了修改，主要用于学习图形学算法，感谢大佬为后人提供的便利。
* 本文每个文件夹中的README展示了我个人对于该项目的学习，理解以及自己实现后的场景效果，希望可以帮到在图形学的学习过程中的你。

# 项目目录
1. __RSM  (Reflective_Shadow_Map)__
2. __Non-interleaved Deferred Shading of Interleaved Sample__
3. 随机光源裁剪  __SLC  (Stochastic Light Culling)__
4. 改进的交错采样  __Metalights Improved Interleaved Shading__
5. 多级渲染顺序无关的透明度算法  __Interactive Order Independent Transparency__
6. 王氏纹理  __Wang Tiles for Image and Texture Generation__
7. 分块着色  __Tile Shading__
8. 风格化渲染  __A Non-Photorealistic Lighting Model__
9. 球谐光照  __Spherical Harmonic Lighting The Gritty Details__
10. 面光源  __LTC (Realtime Polygonal Light Shading With Linearly Transformed Cosine)__
11. 改进的球谐光照  (将法线也进行预计算)  __An Efficient Representation for Irradiance Environment Maps__
12. 基于像素链表的OIT算法  __Real Time Concurrent Linked List Construction on the GPU__
13. (未完成)  __Fourier Opacity Optimization for Scalable Exploration__
14. __SSAO & SSDO  (Approximating Dynamic Global Illumination in Image Space)__
15. __HBAO  (Image Space Horizon Based Ambient Occlusion)__
16. (未完成)  __Cascaded Light Propagation Volumes for Real-Time Indirect Illumination__
17. __FLC  (Forward Light Cuts A Scalable Approach to RealTime Global Illumination)__
18. (未完成)  __CSM (Cascaded Shadow Maps)__
19. 八面体映射  (天空盒压缩)  __A Survey of Efficient Representations  for Independent Unit Vectors__
20. 盒型滤波  __Real-time Approximation to Large Convolution Kernel__
21. Monica大佬的论文  __Real-time indirect illumination by virtual planar area lights__
22. __VSM  (Variance Shadow Maps)__
23. __SSRT  (Efficient GPU Screen-Space Ray Tracing)__
24. (未完成)  __Real-Time Global Illumination using Precomputed Light Field Probes__
25. __ESP  (Exponential Shadow Maps)__


# 项目配置

* 首先克隆本项目到本地，也可以使用git lfs clone指令克隆到本地。

* 下载GraphicSDK https://pan.baidu.com/s/1XjYGrGhCO_KSqL7pCUzBrA 密码: 1234 ，然后将其解压即可，这是Monica大佬配好的环境，包含的各种库也可以进一步学习如何使用。

* 用**管理员的权限**运行GraphicSDK里面的 Graphic.bat 以自动配置环境变量

* 在克隆好的项目中点击setup.bat生成vs环境（默认生成VS2019，可以用文本格式打开setup.bat用你自己的vs版本替代vs2019保存即可）

* 在项目中选择Frame点击生成以生成动态链接库，Monica大佬提供了一个比较完备的OpenGL封装库，在我学习的过程中提供了非常大的帮助，在这里再次感谢大佬。

* 然后就可以运行其他的项目了~