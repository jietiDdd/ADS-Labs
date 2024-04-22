import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']

# 准备数据
x = ['workload1','worklaod2','workload3']  # x轴数据

y1 = [1833,434,1595]
y2 = [854,423,809]
y3 = [890,549,856]

# 绘制折线图
plt.plot(x, y1, label='RaidxTree')
plt.plot(x, y2, label='CompressedRadixTree')
plt.plot(x, y3, label='RedBlackTree')
# 添加图例
plt.legend()

# 添加标题和轴标签
plt.title("不同工作负载下查找的平均时延")
plt.xlabel("工作负载")
plt.ylabel("平均时延")

# 显示图形
plt.show()