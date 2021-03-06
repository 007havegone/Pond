/* -*-C++-*- */
/*
 * Exceptions.
 *
 * Copyright (C) 2003 Carnegie Mellon University and Rutgers University
 *
 * Permission is hereby granted to distribute this software for
 * non-commercial research purposes, provided that this copyright
 * notice is included with any such distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 * SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 * ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *
 * $Id: exceptions.h,v 1.1 2006/07/11 17:53:06 dan Exp $
 */
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <config.h>
#include <iostream>
#include <string>


/* ====================================================================== */
/* Exception */

/*
 * An exception.
 */
struct Exception {
  /* Constructs a runtime exception. */
  explicit Exception(const std::string& msg);

private:
  /* Message. */
  std::string msg_;

  friend std::ostream& operator<<(std::ostream& os, const Exception& e);
};

/* Output operator for exceptions. */
std::ostream& operator<<(std::ostream& os, const Exception& e);


#endif /* EXCEPTIONS_H */
