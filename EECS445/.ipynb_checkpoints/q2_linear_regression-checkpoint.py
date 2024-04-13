"""
EECS 445 - Introduction to Maching Learning
HW2 Q2 Linear Regression Optimization Methods)
~~~~~~
Follow the instructions in the homework to complete the assignment.
"""

import numpy as np
import matplotlib.pyplot as plt
from helper import load_data
import math 
import time

def calculate_empirical_risk(X, y, theta):
    cur_sum = 0
    for i in range(len(X)):
        cur_sum += ((y[i] - np.matmul(X[i],theta))**2)/2
    E_rms = float(cur_sum / len(X))
    return E_rms


def calculate_RMS_Error(X, y, theta):
    """
    Args:
        X: np.array, shape (n, d) 
        y: np.array, shape (n,)
        theta: np.array, shape (d,). Specifies an (d-1)^th degree polynomial

    Returns:
        E_rms: float. The root mean square error as defined in the assignment.
    """
    sum_e = 0
    for i in range(len(X)):
        sum_e += (np.matmul(X[i],theta)-y[i])**2
    return np.sqrt(sum_e/len(X))


def generate_polynomial_features(X, M):
    """
    Create a polynomial feature mapping from input examples. Each element x
    in X is mapped to an (M+1)-dimensional polynomial feature vector 
    i.e. [1, x, x^2, ...,x^M].

    Args:
        X: np.array, shape (n, 1). Each row is one instance.
        M: a non-negative integer
    
    Returns:
        Phi: np.array, shape (n, M+1)
    """
    n = len(X)
    Phi = np.ones((n, M+1))
    if M == 0:
        return Phi
    for i in range(n):
        Phi[i,1] = X[i]
    for i in range(n):
        for j in range(2,M+1):
            Phi[i,j] = Phi[i,j-1]*X[i]
    return Phi


def ls_gradient_descent(X, y, learning_rate=0):
    """
    Implements the Gradient Descent (GD) algorithm for least squares regression.
    Note:
        - Please use the stopping criteria: number of iterations >= 1e6 or |new_loss - prev_loss| <= 1e-10

    Args:
        X: np.array, shape (n, d) 
        y: np.array, shape (n,)
    
    Returns:
        theta: np.array, shape (d,)
    """
    st = time.time()
    X = generate_polynomial_features(X,1)
    iterations = 0
    theta = np.zeros((X.shape[1],))
    cur_loss = calculate_empirical_risk(X,y,theta)
    while iterations < 1e6 and (abs(cur_loss - calculate_empirical_risk(X,y,theta)) > 1e-10 or iterations == 0):
        cur_loss = calculate_empirical_risk(X,y,theta)
        add = np.zeros((X.shape[1],), dtype='float64')
        for i in range(len(X)):
            add += -(y[i]-float(np.matmul(X[i],theta)))*X[i]
        add /= float(len(X))
        theta -= learning_rate * add
        iterations += 1
    print(iterations)
    et = time.time()
    elapsed_time = et - st
    print(elapsed_time)
    return theta


def ls_stochastic_gradient_descent(X, y, learning_rate=0):
    """
    Implements the Stochastic Gradient Descent (SGD) algorithm for least squares regression.
    Note:
        - Please do not shuffle your data points.
        - Please use the stopping criteria: number of iterations >= 1e6 or |new_loss - prev_loss| <= 1e-10
    
    Args:
        X: np.array, shape (n, d) 
        y: np.array, shape (n,)
    
    Returns:
        theta: np.array, shape (d,)
    """
    st = time.time()
    X = generate_polynomial_features(X,1)
    iterations = 0
    theta = np.zeros((X.shape[1],))
    cur_loss = calculate_empirical_risk(X,y,theta)
    while iterations < 1e6 and (abs(cur_loss - calculate_empirical_risk(X,y,theta)) > 1e-10 or iterations == 0):
        cur_loss = calculate_empirical_risk(X,y,theta)
        for i in range(len(X)):
            if y[i]*(np.matmul(X[i],theta)) < 1:
                theta += learning_rate*((y[i]-np.matmul(theta,X[i]))*X[i])
            iterations += 1
    print(iterations)
    et = time.time()
    elapsed_time = et - st
    print(elapsed_time)
    return theta
    
    



def closed_form_optimization(X, y,reg_param=0):
    """
    Implements the closed form solution for least squares regression.

    Args:
        X: np.array, shape (n, d) 
        y: np.array, shape (n,)
        `reg_param`: float, an optional regularization parameter

    Returns:
        theta: np.array, shape (d,)
    """
    st = time.time()
    theta = np.zeros((X.shape[1],))
    Id = np.identity(X.shape[1])
    theta = np.matmul(np.matmul(np.linalg.pinv(reg_param*len(X)*Id+np.matmul(X.T,X)),X.T),y)
    et = time.time()
    #elapsed_time = et - st
    #print(elapsed_time)
    return theta


def part_2_1(fname_train):
    # TODO: This function should contain all the code you implement to complete 2.1. Please feel free to add more plot commands.
    print("========== Part 2.1 ==========")

    X_train, y_train = load_data(fname_train)
    print(closed_form_optimization(X_train,y_train,1))
    print("Done!")
    plt.plot(X_train, y_train, 'ro')
    plt.legend()
    plt.savefig('q2_1.png', dpi=200)
    plt.close()


def part_2_2(fname_train, fname_validation):
    # TODO: This function should contain all the code you implement to complete 2.2
    print("=========== Part 2.2 ==========")

    X_train, y_train = load_data(fname_train)
    X_validation, y_validation = load_data(fname_validation)
    
    # (a) OVERFITTING

    errors_train = np.zeros((11,))
    errors_validation = np.zeros((11,))
    # Add your code here
    #print(closed_form_optimization(X_train,y_train,0))   
    '''x_plot = []
    y_plot = []
    z_plot = []
    for M in range(0,12):
        X = generate_polynomial_features(X_train,M)
        Z = generate_polynomial_features(X_validation,M)
        print(closed_form_optimization(X,y_train))
        print(calculate_RMS_Error(X,y_train,closed_form_optimization(X,y_train)))
        z_plot.append(calculate_RMS_Error(Z,y_validation,closed_form_optimization(X,y_train)))
        y_plot.append(calculate_RMS_Error(X,y_train,closed_form_optimization(X,y_train)))
        x_plot.append(M)
    plt.plot(x_plot,y_plot)
    plt.plot(x_plot,z_plot)
    plt.savefig("q22ai")
    plt.plot(errors_train,'-or',label='Train')
    plt.plot(errors_validation,'-ob', label='Validation')
    plt.xlabel('M')
    plt.ylabel('$E_{RMS}$')
    plt.title('Part 2.2.a')
    plt.legend(loc=1)
    plt.xticks(np.arange(0, 11, 1))
    plt.savefig('q2_2_a.png', dpi=200)
    plt.close()'''


    # (b) REGULARIZATION

    errors_train = np.zeros((10,))
    errors_validation = np.zeros((10,))
    L = np.append([0], 10.0 ** np.arange(-8, 1))
    # Add your code here
    x_plot = []
    y_plot = []
    z_plot = []
    M = 10
    for l in L:
        X = generate_polynomial_features(X_train,M)
        Z = generate_polynomial_features(X_validation,M)
        z_plot.append(calculate_RMS_Error(Z,y_validation,closed_form_optimization(X,y_train,l)))
        y_plot.append(calculate_RMS_Error(X,y_train,closed_form_optimization(X,y_train,l)))
        x_plot.append(l)
    print("y_plot and z_plot: ")
    print(y_plot)
    print(z_plot)
    plt.plot(x_plot,y_plot)
    plt.plot(x_plot,z_plot)
    plt.xscale('symlog', linthresh=1e-8)
    plt.savefig("q22bii")
    plt.figure()
    plt.plot(L, errors_train, '-or', label='Train')
    plt.plot(L, errors_validation, '-ob', label='Validation')
    plt.xscale('symlog', linthresh=1e-8)
    plt.xlabel('$\lambda$')
    plt.ylabel('$E_{RMS}$')
    plt.title('Part 2.2.b')
    plt.legend(loc=2)
    plt.savefig('q2_2_b.png', dpi=200)
    plt.close()

    print("Done!")


def main(fname_train, fname_validation):
    #part_2_1(fname_train)
    part_2_2(fname_train, fname_validation)

if __name__ == '__main__':
    main("dataset/q2_train.csv", "dataset/q2_validation.csv")
