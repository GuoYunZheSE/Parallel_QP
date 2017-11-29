import pandas as pd
import numpy as np
dataset_path='C:\\Users\\user\\Documents\\Visual Studio 2017\\Projects\\Parallel_QP\\Parallel_QP\\DataSet\\netflix_all.txt.test'
data=pd.read_table(dataset_path,header=['Row','Column','Value'],encoding='utf-8',delim_whitespace=True,index_col=0)