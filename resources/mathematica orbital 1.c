  Function[
    {
      a,e,i,argument_periapsis,longitude_asc_node,mean_anomaly_epoch
    },

    {
      a *
      Cos(argument_periapsis) *
      Cos(longitude_asc_node) *
      (
        -e +
        Cos(
          mean_anomaly_epoch +
          e * Sin(
            mean_anomaly_epoch +
            e * Sin(
              mean_anomaly_epoch +
              e * Sin(
                mean_anomaly_epoch +
                e * Sin(
                  mean_anomaly_epoch
                )
              )
            )
          )
        )
      ) *
      (
        -a *
        Cos(i)
      ) *
      (
        -e +
        Cos(
          mean_anomaly_epoch +
          e * Sin(
            mean_anomaly_epoch +
            e * Sin(
              mean_anomaly_epoch +
              e * Sin(
                mean_anomaly_epoch +
                e * Sin(
                  mean_anomaly_epoch
                )
              )
            )
          )
        )
      ) *
      Sin(argument_periapsis) *
      (
        Sin(longitude_asc_node) -
        (
          a *
          Sqrt(1 - e^2) *
          Cos(longitude_asc_node) *
          Sin(argument_periapsis) *
          Sin(
            mean_anomaly_epoch +
            (
              e * Sin(
                mean_anomaly_epoch +
                e * Sin(
                  mean_anomaly_epoch +
                  e * Sin(
                    mean_anomaly_epoch +
                    e * Sin(
                      mean_anomaly_epoch
                    )
                  )
                )
              )
            )
          )
        )
      ) -
      (
        a *
        Sqrt(1 - e^2) *
        Cos(i) *
        Cos(argument_periapsis) *
        Sin(longitude_asc_node) *
        Sin(
          mean_anomaly_epoch +
          e * Sin(
            mean_anomaly_epoch +
            e * Sin(
              mean_anomaly_epoch +
              e * Sin(
                mean_anomaly_epoch +
                e * Sin(
                  mean_anomaly_epoch
                )
              )
            )
          )
        )
      ),

      a *
      Cos(i) *
      Cos(longitude_asc_node) *
      (
        -e +
        Cos(
          mean_anomaly_epoch +
          e * Sin(
            mean_anomaly_epoch +
            e * Sin(
              mean_anomaly_epoch +
              e Sin(
                mean_anomaly_epoch +
                e Sin(
                  mean_anomaly_epoch
                )
              )
            )
          )
        )
      ) *
      Sin(argument_periapsis) +
      (
        a * Cos(
          argument_periapsis) *
          (
            -e +
            Cos(
              mean_anomaly_epoch +
              e * Sin(
                mean_anomaly_epoch +
                e * Sin(
                  mean_anomaly_epoch +
                  e * Sin(
                    mean_anomaly_epoch +
                    e * Sin(
                      mean_anomaly_epoch
                    )
                  )
                )
              )
            )
          ) *
          Sin(
            longitude_asc_node) +
            a *
            Sqrt(1 - e^2) *
            Cos(i) *
            Cos(argument_periapsis) *
            Cos(longitude_asc_node) *
            Sin(
              mean_anomaly_epoch +
              e * Sin(
                mean_anomaly_epoch +
                e * Sin(
                  mean_anomaly_epoch +
                  e * Sin(
                    mean_anomaly_epoch +
                    e * Sin(
                      mean_anomaly_epoch
                    )
                  )
                )
              )
            ) -
            (
              a *
              Sqrt(1 - e^2) *
              Sin(argument_periapsis) *
              Sin(longitude_asc_node) *
              Sin(
                mean_anomaly_epoch +
                e * Sin(
                  mean_anomaly_epoch +
                  e * Sin(
                    mean_anomaly_epoch +
                    e * Sin(
                      mean_anomaly_epoch +
                      e * Sin(
                        mean_anomaly_epoch
                      )
                    )
                  )
                )
              )
            )
          )
        )
      a (-e + Cos(mean_anomaly_epoch + e Sin(mean_anomaly_epoch + e Sin(mean_anomaly_epoch + e Sin(mean_anomaly_epoch + e Sin(mean_anomaly_epoch)))))) Sin(i) Sin(argument_periapsis) + a Sqrt(1-e^2) Cos(argument_periapsis) Sin(i) Sin(mean_anomaly_epoch + e Sin(mean_anomaly_epoch + e Sin(mean_anomaly_epoch + e Sin(mean_anomaly_epoch + e Sin(mean_anomaly_epoch)))))
    }
  ]
