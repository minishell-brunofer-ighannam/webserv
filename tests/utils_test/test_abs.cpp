/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_abs.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 13:32:37 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/06/18 13:36:52 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <limits>

#include "utils.hpp"
#include "test_utils.hpp"

// ── abs<T> ────────────────────────────────────────────────────────────────────

// Feature: abs de inteiro positivo retorna o próprio valor
int test_abs_positive_int()
{
	return assert_true(utils::abs(5) == 5, "test_abs_positive_int", LINE_DATA());
}

// Feature: abs de inteiro negativo retorna o valor positivo
int test_abs_negative_int()
{
	return assert_true(utils::abs(-5) == 5, "test_abs_negative_int", LINE_DATA());
}

// Feature: abs de zero retorna zero
int test_abs_zero()
{
	return assert_true(utils::abs(0) == 0, "test_abs_zero", LINE_DATA());
}

// Feature: abs de double positivo/negativo
int test_abs_double()
{
	bool ok = utils::abs(3.14) == 3.14 && utils::abs(-3.14) == 3.14;
	return assert_true(ok, "test_abs_double", LINE_DATA());
}

// BUG: abs(entry) faz "entry > 0 ? entry : -entry". Para o menor valor
// representável de um tipo com sinal (ex.: INT_MIN), "-entry" é overflow
// (undefined behavior em C++, pois -INT_MIN não é representável em int).
// Na prática, na maioria das implementações com overflow de complemento de
// dois, o resultado "estoura" e retorna o PRÓPRIO INT_MIN (negativo), em vez
// do valor positivo esperado.
// Comportamento esperado: abs(INT_MIN) deveria, idealmente, ou ser evitado
// (documentado como entrada inválida) ou usar um tipo de retorno maior para
// representar o resultado corretamente.
int test_abs_int_min_overflow_BUG()
{
	int result = utils::abs(std::numeric_limits<int>::min());
	// Comportamento atual nesta plataforma: o overflow de "-INT_MIN" devolve
	// o próprio INT_MIN (ainda negativo), não o valor absoluto positivo.
	return assert_true(result == std::numeric_limits<int>::min(),
					"test_abs_int_min_overflow_BUG", LINE_DATA());
}

// ── main ──────────────────────────────────────────────────────────────────────

int main(int argc, char **argv)
{
	messages::settup(argc, argv);

	int failures = 0;


	// std::cout << "\n=== abs<T> ===\n";
	failures += test_abs_positive_int();
	failures += test_abs_negative_int();
	failures += test_abs_zero();
	failures += test_abs_double();
	failures += test_abs_int_min_overflow_BUG();

	messages::print();

	return failures;
}
