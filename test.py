import numpy as np
import matplotlib.pyplot as plt

# 真实值和预测值
y_true = np.array([1, 2, 3, 4, 5])
y_pred = np.array([1.2, 2.5, 3.7, 4.1, 5.3])

# 计算均方误差
mse = np.mean((y_true - y_pred) ** 2)
print("MSE:", mse)

# 绘制真实值和预测值的散点图
plt.scatter(y_true, y_pred)
plt.plot([min(y_true), max(y_true)], [min(y_true), max(y_true)], 'k--', lw=2)  # 绘制直线y=x
plt.xlabel('True Values')
plt.ylabel('Predicted Values')
plt.title('Scatter plot of True vs Predicted Values')
plt.show()