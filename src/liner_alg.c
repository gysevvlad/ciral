#include "liner_alg.h"

/* предполагается, что система невырождена */ 
matrix_t* gauss( matrix_t *X ) {
	matrix_t *answ;
	matrix_t *tmp;
	int *index,
	    max_i,
	    i, j, k,
	    col = X->col_count,
	    row = X->row_count;
	double max, c, sum;

	/* вот тут, по-хорошему, надо посчитать определитель */

	index = (int*) malloc( X->row_count * sizeof(int) );
	for ( i = 0; i < X->row_count; i++ ) 
		index[i] = -1;

	answ = matrix_create( row, 1 );

	for( i = 1; i < row; i++ ) {

		/* partial pivoting */
		max = fabs(*matrix_at( X, i-1, i-1 ));
		max_i = i-1;
		for ( j = i; j < row; j++ )
			if ( max < fabs(*matrix_at( X, j, i-1 )))
				max = fabs(*matrix_at( X, i-1, j )), max_i = j;
		matrix_row_swap( X, i-1, max_i );
		index[i-1] = max_i;
		index[max_i] = i - 1;

		if ( max == 0 ) 
			return NULL;

		for ( j = i; j < row; j++ ) {
			if (*matrix_at( X, j, i-1 ) == 0.0) 
				c = 0.0;
			else
				c = -(*matrix_at( X, j, i-1 ))/(*matrix_at( X, i-1, i-1 ));
			for ( k = i-1; k < col; k++ )
				*matrix_at( X, j, k ) += c*(*matrix_at( X, i-1, k ));
		}
	}

	for ( i = row-1; i >= 0; i-- ) {
		sum = 0;
		for ( j = i + 1; j < col-1; j++ )
			sum += (*matrix_at( X, i, j ))*(*matrix_at( answ, j, 0 ));
		*matrix_at( answ, i, 0 ) = ((*matrix_at( X, i, col-1))-sum)/(*matrix_at( X, i, i));
	}

	tmp = matrix_create( row, 1 );
		
	for ( i = 0; i < row; i++ ) {
		if ( index[i] == -1 ) 
			*matrix_at( tmp, i, 0 ) = *matrix_at( answ, i, 0 );
		else
			*matrix_at( tmp, i, 0 ) = *matrix_at( answ, index[i], 0 );
	}
	return answ;
}