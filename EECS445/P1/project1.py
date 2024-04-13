"""EECS 445 - Winter 2023.

Project 1
"""

import pandas as pd
import numpy as np
import itertools
import string
import random
import math

from sklearn import svm
from sklearn.svm import SVC, LinearSVC
from sklearn.model_selection import StratifiedKFold
from sklearn import metrics
from matplotlib import pyplot as plt


from helper import *

import warnings
from sklearn.exceptions import ConvergenceWarning

warnings.simplefilter(action="ignore", category=FutureWarning)
warnings.simplefilter(action="ignore", category=ConvergenceWarning)

np.random.seed(445)



def extract_word(input_string):
    """Preprocess review into list of tokens.

    Convert input string to lowercase, replace punctuation with spaces, and split along whitespace.
    Return the resulting array.

    E.g.
    > extract_word("I love EECS 445. It's my favorite course!")
    > ["i", "love", "eecs", "445", "it", "s", "my", "favorite", "course"]

    Input:
        input_string: text for a single review
    Returns:
        a list of words, extracted and preprocessed according to the directions
        above.
    """
    for i in input_string:
        if i in string.punctuation:
            temp = ""
            temp += i
            input_string = input_string.replace(temp, "")
            input_string = input_string.lower()
    return input_string.split()


def extract_dictionary(df):
    """Map words to index.

    Reads a pandas dataframe, and returns a dictionary of distinct words
    mapping from each distinct word to its index (ordered by when it was
    found).

    E.g., with input:
        | text                          | label | ... |
        | It was the best of times.     |  1    | ... |
        | It was the blurst of times.   | -1    | ... |

    The output should be a dictionary of indices ordered by first occurence in
    the entire dataset:
        {
           it: 0,
           was: 1,
           the: 2,
           best: 3,
           of: 4,
           times: 5,
           blurst: 6
        }
    The index should be autoincrementing, starting at 0.

    Input:
        df: dataframe/output of load_data()
    Returns:
        a dictionary mapping words to an index
    """
    word_dict = {}
    n = len(df)
    for i in range(n):
        curr_row = df['text'][i]
        curr_row = extract_word(curr_row)
        for j in curr_row:
            if j in word_dict:
                temp = word_dict[j]
                word_dict[j] = temp+1
            if j not in word_dict:
                word_dict[j] = 1  
    return word_dict


def generate_feature_matrix(df, word_dict):
    """Create matrix of feature vectors for dataset.

    Reads a dataframe and the dictionary of unique words to generate a matrix
    of {1, 0} feature vectors for each review.  Use the word_dict to find the
    correct index to set to 1 for each place in the feature vector. The
    resulting feature matrix should be of dimension (# of reviews, # of words
    in dictionary).

    Input:
        df: dataframe that has the text and labels
        word_dict: dictionary of words mapping to indices
    Returns:
        a numpy matrix of dimension (# of reviews, # of words in dictionary)
    """
    counter = 0;
    number_of_reviews = df.shape[0]
    number_of_words = len(word_dict)
    feature_matrix = np.zeros((number_of_reviews, number_of_words))
    for i in range(number_of_reviews):
        curr_rev = extract_word(df['text'][i])
        for j in curr_rev:
            if j in word_dict:
                found = list(word_dict).index(j)
                feature_matrix[i][found] = 1
    return feature_matrix

def generate_mult_feature_matrix(df, word_dict):
    """Create matrix of feature vectors for dataset.

    Reads a dataframe and the dictionary of unique words to generate a matrix
    of {1, 0} feature vectors for each review.  Use the word_dict to find the
    correct index to set to 1 for each place in the feature vector. The
    resulting feature matrix should be of dimension (# of reviews, # of words
    in dictionary).

    Input:
        df: dataframe that has the text and labels
        word_dict: dictionary of words mapping to indices
    Returns:
        a numpy matrix of dimension (# of reviews, # of words in dictionary)
    """
    counter = 0;
    number_of_reviews = df.shape[0]
    number_of_words = len(word_dict)
    feature_matrix = np.zeros((number_of_reviews, number_of_words))
    for i in range(number_of_reviews):
        curr_rev = extract_word(df['text'][i])
        for j in curr_rev:
            if j in word_dict:
                found = list(word_dict).index(j)
                feature_matrix[i][found] = 1*math.log(len(curr_rev))
    return feature_matrix


def performance(y_true, y_pred, metric="accuracy"):
    """Calculate performance metrics.

    Performance metrics are evaluated on the true labels y_true versus the
    predicted labels y_pred.

    Input:
        y_true: (n,) array containing known labels
        y_pred: (n,) array containing predicted scores
        metric: string specifying the performance metric (default='accuracy'
                 other options: 'f1-score', 'auroc', 'precision', 'sensitivity',
                 and 'specificity')
    Returns:
        the performance as an np.float64
    """
    cv = metrics.confusion_matrix(y_true,y_pred)
    tp = cv[0,0]
    fp = cv[0,1]
    fn = cv[1,0]
    tn = cv[1,1]
    if metric == "accuracy":
        if tp+fp+fn+tn == 0:
            return 0
        else:
            return (tp+tn)/(tp+tn+fp+fn)
    if metric == "precision":
        if tp+fp == 0:
            return 0
        else:
            return tp/(tp+fp)
    if metric == "sensitivity":
        if tp+fn == 0:
            return 0
        else:
            return tp/(tp+fn)
    if metric == "specificity":
        if tn+fp == 0:
            return 0
        else:
            return tn/(tn+fp)
    if metric == "f1-score":
        if tp+fn == 0 or tn+fp == 0:
            return 0
        if tp/(tp+fn) + tn/(tn+fp) == 0:
            return 0
        else:
            return 2*(tp/(tp+fn))*(tn/(tn+fp))/(tp/(tp+fn) + tn/(tn+fp))
    else:
        return 0
    
    # TODO: Implement this function
    # This is an optional but very useful function to implement.
    # See the sklearn.metrics documentation for pointers on how to implement
    # the requested metrics.


def cv_performance(clf, X, y, k=5, metric="accuracy"):
    """Split data into k folds and run cross-validation.

    Splits the data X and the labels y into k-folds and runs k-fold
    cross-validation: for each fold i in 1...k, trains a classifier on
    all the data except the ith fold, and tests on the ith fold.
    Calculates and returns the k-fold cross-validation performance metric for
    classifier clf by averaging the performance across folds.
    Input:
        clf: an instance of SVC()
        X: (n,d) array of feature vectors, where n is the number of examples
           and d is the number of features
        y: (n,) array of binary labels {1,-1}
        k: an int specifying the number of folds (default=5)
        metric: string specifying the performance metric (default='accuracy'
             other options: 'f1-score', 'auroc', 'precision', 'sensitivity',
             and 'specificity')
    Returns:
        average 'test' performance across the k folds as np.float64
    """
    # TODO: Implement this function
    # HINT: You may find the StratifiedKFold from sklearn.model_selection
    # to be useful

    # Put the performance of the model on each fold in the scores array
    scores = []
    skf = StratifiedKFold(n_splits=k)
    for i, (train_index, test_index) in enumerate(skf.split(X, y)):
        xtrain = X.iloc[train_index]
        ytrain = y.iloc[train_index]
        ytrue = y.iloc[test_index]
        clf.fit(xtrain, ytrain)
        if metric == "auroc":
            ypred = clf.decision_function(X.iloc[test_index])
            scores.append(metrics.roc_auc_score(ytrue,ypred))
        else:
            ypred = clf.predict(X.iloc[test_index])
            scores.append(performance(ytrue,ypred,metric))
    return np.array(scores).mean()


def select_param_linear(X, y, k=5, metric="accuracy", C_range=[], loss="hinge", 
                        penalty="l2", dual=True):
    """Search for hyperparameters from the given candidates of linear SVM with 
    best k-fold CV performance.

    Sweeps different settings for the hyperparameter of a linear-kernel SVM,
    calculating the k-fold CV performance for each setting on X, y.
    Input:
        X: (n,d) array of feature vectors, where n is the number of examples
        and d is the number of features
        y: (n,) array of binary labels {1,-1}
        k: int specifying the number of folds (default=5)
        metric: string specifying the performance metric (default='accuracy',
             other options: 'f1-score', 'auroc', 'precision', 'sensitivity',
             and 'specificity')
        C_range: an array with C values to be searched over
        loss: string specifying the loss function used (default="hinge",
             other option of "squared_hinge")
        penalty: string specifying the penalty type used (default="l2",
             other option of "l1")
        dual: boolean specifying whether to use the dual formulation of the
             linear SVM (set True for penalty "l2" and False for penalty "l1"ß)
    Returns:
        the parameter value for a linear-kernel SVM that maximizes the
        average 5-fold CV performance.
    """
    maxsofar = 0
    maxi = 0
    for i in C_range:
        clf = LinearSVC(penalty=penalty, loss=loss, dual=dual, C=i, random_state=445)
        curperf = cv_performance(clf,X,y,k,metric)
        if curperf > maxsofar:
            maxsofar = curperf
            maxi = i
    return maxi
        
    # TODO: Implement this function
    # HINT: You should be using your cv_performance function here
    # to evaluate the performance of each SVM


def plot_weight(X, y, penalty, C_range, loss, dual):
    """Create a plot of the L0 norm learned by a classifier for each C in C_range.

    Input:
        X: (n,d) array of feature vectors, where n is the number of examples
        and d is the number of features
        y: (n,) array of binary labels {1,-1}
        penalty: string for penalty type to be forwarded to the LinearSVC constructor
        C_range: list of C values to train a classifier on
        loss: string for loss function to be forwarded to the LinearSVC constructor
        dual: whether to solve the dual or primal optimization problem, to be
            forwarded to the LinearSVC constructor
    Returns: None
        Saves a plot of the L0 norms to the filesystem.
    """
    norm0 = []
    for i in C_range:
        clf = LinearSVC(penalty=penalty, loss=loss, dual=dual, C=i, random_state=445)
        coef = clf.fit(X,y).coef_
        curr_norm = 0
        for i in coef[0]:
            if i != 0:
                curr_norm = curr_norm + 1
        print(curr_norm)
        norm0.append(curr_norm)
    # TODO: Implement this part of the function
    # Here, for each value of c in C_range, you should
    # append to norm0 the L0-norm of the theta vector that is learned
    # when fitting an L2- or L1-penalty, degree=1 SVM to the data (X, y)

    plt.plot(C_range, norm0)
    plt.xscale("log")
    plt.legend(["L0-norm"])
    plt.xlabel("Value of C")
    plt.ylabel("Norm of theta")
    plt.title("Norm-" + penalty + "_penalty.png")
    plt.savefig("Norm-" + penalty + "_penalty.png")
    plt.close()


def select_param_quadratic(X, y, k=5, metric="accuracy", param_range=[]):
    """Search for hyperparameters from the given candidates of quadratic SVM 
    with best k-fold CV performance.

    Sweeps different settings for the hyperparameters of an quadratic-kernel SVM,
    calculating the k-fold CV performance for each setting on X, y.
    Input:
        X: (n,d) array of feature vectors, where n is the number of examples
           and d is the number of features
        y: (n,) array of binary labels {1,-1}
        k: an int specifying the number of folds (default=5)
        metric: string specifying the performance metric (default='accuracy'
                 other options: 'f1-score', 'auroc', 'precision', 'sensitivity',
                 and 'specificity')
        param_range: a (num_param, 2)-sized array containing the
            parameter values to search over. The first column should
            represent the values for C, and the second column should
            represent the values for r. Each row of this array thus
            represents a pair of parameters to be tried together.
    Returns:
        The parameter values for a quadratic-kernel SVM that maximize
        the average 5-fold CV performance as a pair (C,r)
    """
    maxsofar = 0
    maxiC = 0
    maxiR = 0
    for i in param_range[:,0]:
        for j in param_range[:,1]:
            print(i,j)
            clf = SVC(kernel="poly", degree=2, C=i, coef0=j, gamma="auto")
            curperf = cv_performance(clf,X,y,k,metric)
            if curperf > maxsofar:
                maxsofar = curperf
                maxiC = i
                maxiR = j
    print(maxsofar)         
    '''for i in range(25):
        c = random.uniform(-2,3)
        r = random.uniform(-2,3)
        clf = SVC(kernel="poly", degree=2, C=10**c, coef0=10**r, gamma="auto")
        curperf = cv_performance(clf,X,y,k,metric)
        if curperf > maxsofar:
            maxsofar = curperf
            maxiC = 10**c
            maxiR = 10**r'''
    return (maxiC,maxiR)
    # TODO: Implement this function
    # Hint: This will be very similar to select_param_linear, except
    # the type of SVM model you are using will be different...




def main():
    # Read binary data
    # NOTE: READING IN THE DATA WILL NOT WORK UNTIL YOU HAVE FINISHED
    #       IMPLEMENTING generate_feature_matrix AND extract_dictionary
    X_train, Y_train, X_test, Y_test, dictionary_binary = get_split_binary_data(
       fname="data/dataset.csv"
    )

    #print(max(dictionary_binary, key=dictionary_binary.get))
    '''multiclass_features, multiclass_labels, multi_dictionary = get_multiclass_training_data()
    multiclass_features = pd.DataFrame(multiclass_features)
    multiclass_labels = pd.DataFrame(multiclass_labels)
    multiclass_test = get_heldout_reviews(multi_dictionary)
    lin_mult = svm.LinearSVC(tol=1e-6, C=0.1)
    lin_mult.fit(multiclass_features,multiclass_labels)
    ymult_pred = lin_mult.predict(multiclass_features)
    generate_challenge_labels(ymult_pred, "ellench")
    print(cv_performance(lin_mult, multiclass_features, multiclass_labels, k=5, metric="accuracy"))'''
    #IMB_features, IMB_labels, IMB_test_features, IMB_test_labels = get_imbalanced_data(
    #    dictionary_binary, fname="data/dataset.csv"
    #)
    X_train = pd.DataFrame(X_train)
    Y_train = pd.DataFrame(Y_train)
    #X_imb_train = pd.DataFrame(IMB_features)
    #Y_imb_train = pd.DataFrame(IMB_labels)
    #print(select_param_quadratic(X_train, Y_train, k=5, metric="auroc",   
                                 #param_range=np.array([[149.59986710577024,3.7305439537155047]])))
    #clf = LinearSVC(penalty="l2", loss="hinge", dual=True, C=0.01, class_weight={-1: 1, 1: 10}, random_state=445)
    #clf.fit(X_train,Y_train)
    #ypred = clf.predict(X_test)
    #ytrue = Y_test
    #print(performance(ytrue,ypred,metric="specificity"))
    #clf = LinearSVC(penalty="l2", loss="hinge", dual=True, C=0.01, class_weight={-1: 50, 1: 5000}, random_state=445)
    #clf_ = LinearSVC(penalty="l2", loss="hinge", dual=True, C=0.01, class_weight={-1: 1, 1: 1}, random_state=445)
    #clf.fit(X_imb_train, Y_imb_train)
    #clf_.fit(X_imb_train, Y_imb_train)
    #y_test_pred = clf.decision_function(IMB_test_features)
    #y_test_pred_ = clf_.decision_function(IMB_test_features)
    #train_fpr, train_tpr, tr_thresholds = metrics.roc_curve(IMB_test_labels, y_test_pred)
    #test_fpr, test_tpr, te_thresholds = metrics.roc_curve(IMB_test_labels, y_test_pred_)
    #plt.grid()
    #plt.plot(train_fpr, train_tpr, label=" w/ 50 and 5000 ")
    #plt.plot(test_fpr, test_tpr, label=" w/ 1 and 1")
    #plt.plot([0,1],[0,1],'g--')
    #plt.legend()
    #plt.xlabel("False Positive Rate")
    #plt.ylabel("True Positive Rate")
    #plt.title("AUC(ROC curve)")
    #plt.grid(color='black', linestyle='-', linewidth=0.5)
    #plt.savefig("pls_graph")
    #plt.close()
    '''print(select_param_linear(X_train, Y_train, k=5, metric="auroc", C_range=[0.001,0.01,0.1,1,10,100,1000], loss="squared_hinge", penalty="l1", dual=False))
    clf = LinearSVC(penalty="l1", loss="squared_hinge", dual=False, C=1, random_state=445)
    clf.fit(X_train,Y_train)
    ypred = clf.decision_function(X_test)
    ytrue = Y_test
    print(metrics.roc_auc_score(ytrue,ypred))'''
    #plot_weight(X_train,Y_train, penalty="l1", C_range=[0.001,0.01,0.1,1,10,100,1000], loss="squared_hinge", dual=False)
    clf = LinearSVC(penalty="l2", loss="hinge", dual=True, C=0.01, random_state=445)
    coef = clf.fit(X_train,Y_train).coef_
    real = list(coef[0])
    coeff = list(coef[0])
    coeff.sort(reverse=False)
    xplot = []
    yplot = []
    xplot.append(list(dictionary_binary)[real.index(coeff[0])])
    xplot.append(list(dictionary_binary)[real.index(coeff[1])])
    xplot.append(list(dictionary_binary)[real.index(coeff[2])])
    xplot.append(list(dictionary_binary)[real.index(coeff[3])])
    xplot.append(list(dictionary_binary)[real.index(coeff[4])])
    yplot.append(coeff[0])
    yplot.append(coeff[1])
    yplot.append(coeff[2])
    yplot.append(coeff[3])
    yplot.append(coeff[4])
    coeff.sort(reverse=True)
    xplot.append(list(dictionary_binary)[real.index(coeff[0])])
    xplot.append(list(dictionary_binary)[real.index(coeff[1])])
    xplot.append(list(dictionary_binary)[real.index(coeff[2])])
    xplot.append(list(dictionary_binary)[real.index(coeff[3])])
    xplot.append(list(dictionary_binary)[real.index(coeff[4])])
    yplot.append(coeff[0])
    yplot.append(coeff[1])
    yplot.append(coeff[2])
    yplot.append(coeff[3])
    yplot.append(coeff[4])

    plt.bar(xplot,yplot, width = 0.4)
    plt.savefig("word_plot")
    return
    # TODO: Questions 3, 4, 5
    
    # Read multiclass data
    # TODO: Question 6: Apply a classifier to heldout features, and then use
    #       generate_challenge_labels to print the predicted labels
    
    (multiclass_features,
    multiclass_labels,
    multiclass_dictionary) = get_multiclass_training_data()
    
    heldout_features = get_heldout_reviews(multiclass_dictionary)


if __name__ == "__main__":
    main()
