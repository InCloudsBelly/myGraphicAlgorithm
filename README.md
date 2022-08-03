[![GitHub stars](https://img.shields.io/github/stars/MustNotBeGreasy/myGraphicAlgorithm.svg?style=flat-square)](https://github.com/MustNotBeGreasy/myGraphicAlgorithm/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/MustNotBeGreasy/myGraphicAlgorithm?style=flat-square)](https://github.com/MustNotBeGreasy/myGraphicAlgorithm/issues)
[![GitHub forks](https://img.shields.io/github/forks/MustNotBeGreasy/myGraphicAlgorithm?style=flat-square)](https://github.com/MustNotBeGreasy/myGraphicAlgorithm/network)
[![GitHub last commit](https://img.shields.io/github/last-commit/MustNotBeGreasy/myGraphicAlgorithm?style=flat-square)](https://github.com/MustNotBeGreasy/myGraphicAlgorithm)


本项目为对部分图形学论文的复现，主要代码框架和思路参考了AngelMonica126的图形学论文复现的开源项目https://github.com/AngelMonica126/GraphicAlgorithm ，在其基础上加入了自己的理解，并对部分公式参考原论文进行了修改，主要用于学习图形学算法，感谢大佬为后人提供的便利。

# 项目配置

* 首先克隆本项目到本地，也可以使用git lfs clone指令克隆到本地。

* 下载GraphicSDK https://pan.baidu.com/s/1XjYGrGhCO_KSqL7pCUzBrA 密码: 1234 ，然后将其解压即可，这是Monica大佬配好的环境，包含的各种库也可以进一步学习如何使用。

* 用**管理员的权限**运行GraphicSDK里面的 Graphic.bat 以自动配置环境变量

* 在克隆好的项目中点击setup.bat生成vs环境（默认生成VS2019，可以用文本格式打开setup.bat用你自己的vs版本替代vs2019保存即可）

* 在项目中选择Frame点击生成以生成动态链接库，Monica大佬提供了一个比较完备的OpenGL封装库，在我学习的过程中提供了非常大的帮助，在这里再次感谢大佬。

* 然后就可以运行其他的项目了~
