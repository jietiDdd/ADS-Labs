import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']

# 准备数据
x = ['insert','remove','find']  # x轴数据

y1 = [2427,2367,1595]
y2 = [1042,1033,809]
y3 = [973,1029,856]

# 绘制折线图
plt.plot(x, y1, label='RadixTree')
plt.plot(x, y2, label='CompressedRadixTree')
plt.plot(x, y3, label='RedBlackTree')
# 添加图例
plt.legend()

# 添加标题和轴标签
plt.title("workload3下不同操作的时延")
plt.xlabel("操作类型")
plt.ylabel("平均时延")

# 显示图形
plt.show()