-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 07-09-2019 a las 10:16:18
-- Versión del servidor: 10.1.38-MariaDB
-- Versión de PHP: 7.3.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `alergias`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `alergenos`
--

CREATE TABLE `alergenos` (
  `idalergia` int(11) NOT NULL,
  `Descripcion` varchar(45) COLLATE utf8_spanish2_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish2_ci;

--
-- Volcado de datos para la tabla `alergenos`
--

INSERT INTO `alergenos` (`idalergia`, `Descripcion`) VALUES
(1, 'PESCADO'),
(2, 'FRUTOS_SECOS'),
(3, 'LACTEOS'),
(4, 'MOLUSCOS'),
(5, 'GLUTEN'),
(6, 'CRUSTACEOS'),
(7, 'HUEVOS'),
(8, 'CACAHUETES'),
(9, 'SOJA'),
(10, 'APIO'),
(11, 'MOSTAZA'),
(12, 'SESAMO'),
(13, 'ALTRAMUZ'),
(14, 'SULFATOS'),
(15, 'NINGUNA');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `alergia_diaria`
--

CREATE TABLE `alergia_diaria` (
  `id_fecha` int(20) NOT NULL,
  `fecha` date NOT NULL,
  `Alergia_diaria` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish2_ci;

--
-- Volcado de datos para la tabla `alergia_diaria`
--

INSERT INTO `alergia_diaria` (`id_fecha`, `fecha`, `Alergia_diaria`) VALUES
(1, '2019-08-29', 5),
(2, '2019-08-29', 2);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `alumnos`
--

CREATE TABLE `alumnos` (
  `idalumno` int(10) NOT NULL,
  `codigo_alumno` int(10) NOT NULL,
  `Nombre` varchar(45) COLLATE utf8_spanish2_ci DEFAULT NULL,
  `Apellido1` varchar(45) COLLATE utf8_spanish2_ci DEFAULT NULL,
  `Apellido2` varchar(45) COLLATE utf8_spanish2_ci DEFAULT NULL,
  `PAGO` varchar(1) COLLATE utf8_spanish2_ci DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish2_ci;

--
-- Volcado de datos para la tabla `alumnos`
--

INSERT INTO `alumnos` (`idalumno`, `codigo_alumno`, `Nombre`, `Apellido1`, `Apellido2`, `PAGO`) VALUES
(1, 1, 'Julia', 'Miguelez', 'Fernandez', '1'),
(2, 2, 'Adrian', 'Navazo', 'Escudero', '0'),
(3, 3, 'Ignacio', 'Hidalgo', 'Perez', '1');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tiene_alergia`
--

CREATE TABLE `tiene_alergia` (
  `idtiene_alergia` int(10) NOT NULL,
  `codigo_alumno` int(10) DEFAULT NULL,
  `codigo_alergia` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish2_ci;

--
-- Volcado de datos para la tabla `tiene_alergia`
--

INSERT INTO `tiene_alergia` (`idtiene_alergia`, `codigo_alumno`, `codigo_alergia`) VALUES
(1, 1, 1),
(2, 1, 5),
(4, 3, 15),
(5, 1, 13),
(6, 1, 2),
(7, 1, 6),
(8, 1, 9),
(9, 1, 3),
(10, 1, 4),
(11, 2, 15);

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `alergenos`
--
ALTER TABLE `alergenos`
  ADD PRIMARY KEY (`idalergia`);

--
-- Indices de la tabla `alergia_diaria`
--
ALTER TABLE `alergia_diaria`
  ADD PRIMARY KEY (`id_fecha`),
  ADD KEY `alergias_del_dia_idx` (`Alergia_diaria`);

--
-- Indices de la tabla `alumnos`
--
ALTER TABLE `alumnos`
  ADD PRIMARY KEY (`idalumno`),
  ADD UNIQUE KEY `codigo_alumno` (`codigo_alumno`);

--
-- Indices de la tabla `tiene_alergia`
--
ALTER TABLE `tiene_alergia`
  ADD PRIMARY KEY (`idtiene_alergia`),
  ADD KEY `alumno_con_alergia_idx` (`codigo_alergia`),
  ADD KEY `alumno_con_alergia_idx1` (`codigo_alumno`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `alergenos`
--
ALTER TABLE `alergenos`
  MODIFY `idalergia` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=16;

--
-- AUTO_INCREMENT de la tabla `alergia_diaria`
--
ALTER TABLE `alergia_diaria`
  MODIFY `id_fecha` int(20) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT de la tabla `alumnos`
--
ALTER TABLE `alumnos`
  MODIFY `idalumno` int(10) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT de la tabla `tiene_alergia`
--
ALTER TABLE `tiene_alergia`
  MODIFY `idtiene_alergia` int(10) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla `alergia_diaria`
--
ALTER TABLE `alergia_diaria`
  ADD CONSTRAINT `alergias_del_dia` FOREIGN KEY (`Alergia_diaria`) REFERENCES `alergenos` (`idalergia`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Filtros para la tabla `tiene_alergia`
--
ALTER TABLE `tiene_alergia`
  ADD CONSTRAINT `alergia_del_alumno` FOREIGN KEY (`codigo_alergia`) REFERENCES `alergenos` (`idalergia`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `tiene_alergia_ibfk_1` FOREIGN KEY (`codigo_alumno`) REFERENCES `alumnos` (`codigo_alumno`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
