# -*- coding: utf-8 -*-
# Tudor Berariu, 2014

def learning_rate(iter_no, iter_count):
    """Calculează rata de învățare în funcție de iterația curentă și
    numărul total de iterații"""
    ## Exercițiul 2: calculul coeficientului de învățare
    ## Exercițiul 2: completați aici

    MAX_VALUE = 0.75
    MIN_VALUE = 0.1
    lr = 0.0 # TODO
    ## Exercițiul 2: ----------

    lr = MIN_VALUE + (MAX_VALUE - MIN_VALUE) * (float(iter_no) / iter_count)

    return lr
